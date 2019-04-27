/*
Copyright 2019 <Julian Komaromy, Pierre Constantineau>

3-Clause BSD License

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Keyboard.h"

using buffer_t = std::vector<std::pair<uint8_t, uint8_t>>;

namespace Keyboard 
{
    /*
     * the remote layer is the layer from another bluetooth
     * module
     * the current layer is the "correct" one that is being
     * used to scan the keyboard matrix
     * the previous layer is the last "current layer"
     */
    uint8_t remoteLayer = 0;
    uint8_t currentLayer = 0;
    uint8_t previousLayer = 0;

    //TODO merge remote report in

#if KEYBOARD_MODE == HUB || KEYBOARD_MODE == SINGLE
#pragma message "compiling hid report in keyboard"
    /* 
     * the current and previous modifiers, with each
     * bit representing one modifier, e.g. LSHIFT
     */
    uint8_t currentMod = 0;
    uint8_t previousMod = 0;


    std::array<uint8_t, 8> currentReport = {0};
    std::array<uint8_t, 8> previousReport = {0};

    buffer_t momentaryBuffer;
    buffer_t toggleBuffer;
    buffer_t oneshotBuffer;
#else
    std::vector<uint8_t> previousReport;
    std::vector<uint8_t> currentReport;
#endif 

    bool layerChanged = false;

    bool reportEmpty = true;
    bool reportChanged = false;

    unsigned long lastPressed;

    bool emptyOneshot = false;


    std::array<std::array<PKey, MATRIX_COLS>, MATRIX_ROWS> keyboard {{ }};

#if KEYBOARD_MODE == HUB || KEYBOARD_MODE == SINGLE
    void updateModifier(uint8_t modifier)
    {
        currentMod |= 1 << (modifier - KC_LCTRL);
    }
#endif

    void updateLayer(uint8_t layer)
    {
        //the actual virtual layer starts at 0 
        //for LAYER_0, despite the HID keycode being much larger
        layer -= LAYER_0;

        //the layer must be larger than the remote layer to override as
        //well as larger than the current layer, because the largest
        //layer should always be used
        if (layer > remoteLayer && layer > currentLayer)
        {
            currentLayer = layer;
            
            //the layer is changed if the previous layer 
            //isn't the current, updated one
            layerChanged = previousLayer != currentLayer;
        }
    }

    //reset the layer to make sure it bounces 
    //back to zero whenever needed
    void resetLayer()
    {
        if (remoteLayer == 0)
        {
            currentLayer = 0;

            //resetting changes the layer if the previous layer
            //was larger, this can be overridden by 
            layerChanged = previousLayer != 0;
        }
    }

#if KEYBOARD_MODE == HUB || KEYBOARD_MODE == SINGLE
    //add a keycode into the report at a given index and merge extra
    //modifiers into the current modifiers
    void intoReport(uint8_t HID_Keycode, uint8_t extraModifiers, int index)
    {
        if (HID_Keycode >= LAYER_0 && HID_Keycode <= LAYER_F)
        {
            updateLayer(HID_Keycode);
        }
        //if the HID keycode corresponds to a modifier
        else if (HID_Keycode >= KC_LCTRL && HID_Keycode <= KC_RGUI)
        {
            updateModifier(HID_Keycode);
        }
        else 
        {
            //oneshot should be emptied into the report
            //upon a non modifier or layer keycode
            emptyOneshot = true;
            currentReport[index] = HID_Keycode;
        }

        //add extra mods into the current ones, no modifier is 0, 
        //which is identity for logical conjunction, 
        //therefore no change is made if there are no extra modifiers
        currentMod |= extraModifiers;
    }
#endif

    bool handleLayer(uint8_t hid_keycode) 
    {
        if (hid_keycode >= LAYER_0 && hid_keycode <= LAYER_F)
        {
            updateLayer(hid_keycode);    
            return true;
        }

        return false;
    }

    /*
     * sort the keycode of a potentially active
     * key into one of the three buffers on the HUB or SINGLE
     *
     * or add the keycode into the current report on SPOKE
     */
    void intoBuffers(const Keycode& keycode)
    {
        //check if the keycode is even active or has meaning
        if (keycode.isRelevant())
        {
            //extract the information from the keycode
            auto reportPair = keycode.getReportPair();
            auto duration = keycode.getDuration();

#if KEYBOARD_MODE == HUB || KEYBOARD_MODE == SINGLE
            //momentary
            if (duration == 0)
            {
                momentaryBuffer.push_back(reportPair);
            }
            //toggle 
            else if (duration == 1)
            {
                //search for the same pair of extra modifier and keycode
                //in the toggle buffer and remove it if found
                auto it = std::find(toggleBuffer.begin(), toggleBuffer.end(), reportPair); 

                if (it != toggleBuffer.end())
                {
                    toggleBuffer.erase(it);
                }
                else 
                {
                    toggleBuffer.push_back(reportPair);
                }
            }
            //oneshot
            else if (duration == 2)
            {
                oneshotBuffer.push_back(reportPair);
            }
#else
            //check if the keycode is a layer, if so, deal
            //with it according to layer rules, otherwise
            //add it into the report
            if (!handleLayer(keycode.getHIDKeycode()))
            {
                currentReport.push_back(keycode.encode());
            }
#endif
        }
    }

    void updateBuffers(uint8_t layer)
    {
        for (int row = 0; row < MATRIX_ROWS; ++row)
        {
            for (auto& pkey : keyboard[row])
            {
                intoBuffers(pkey.getKeycode(layer));            
            }
        }
    }

    void resetReport()
    {

#if KEYBOARD_MODE == HUB || KEYBOARD_MODE == SINGLE
#pragma message "compiling something for hub/single"
        currentMod = 0;
        currentReport = {0};
#else
        currentReport.clear();
#endif
    }

    void setupKeyboard()
    {
        //run the user defined keymap setup
        //for the definition of the keyboard 
        //matrix
        auto matrix = setupKeymap();

        std::array<VKey, NUM_LAYERS> vkeys;

        //initialize internal matrix from keymap matrix 
        for (int i = 0; i < MATRIX_ROWS; ++i)
        {
            for (int j = 0; j < MATRIX_COLS; ++j)
            {
                for (std::size_t l = 0; l < NUM_LAYERS; ++l)
                {
                    //cast the vkey at the current matrix position
                    //to an rvalue so the shared_ptr actually gets 
                    //moved
                    vkeys[l] = std::move(matrix[l][i][j]); 
                }

                keyboard[i][j] = vkeys;
            }
        }
    }


    void scanMatrix(int currentState, unsigned long currentMillis, int row, int col)
    {
#if DIODE_DIRECTION == COL2ROW
        if (currentState == 0) 
#else
            if (currentState == 1)
#endif 
            {
                //key is pressed
                keyboard[row][col].press(currentMillis, currentLayer);

                //TODO: is there a problem caused by possible changes 
                //upon clear and keyboard going into sleep mode?
                lastPressed = currentMillis;
            }
            else 
            {
                //key is not pressed
                keyboard[row][col].clear(currentMillis, currentLayer);
            }
    }

#if KEYBOARD_MODE == HUB || KEYBOARD_MODE == SINGLE
    std::array<uint8_t, 8> getCurrentReport()
#else
    std::vector<uint8_t> getCurrentReport()
#endif
    {
        //save the current report as the last report
        previousReport = currentReport;
        reportChanged = false;

        return currentReport;
    }

    bool getReportChanged()
    {
        return reportChanged;
    }
    bool getReportEmpty()
    {
        return reportEmpty;
    }

    bool getLayerChanged()
    {
        return layerChanged;
    }

    uint8_t getLocalLayer()
    {
        layerChanged = false;
        return currentLayer;
    }

    unsigned long getLastPressed()
    {
        return lastPressed;
    }

    void updateReport()
    {
#if KEYBOARD_MODE == HUB || KEYBOARD_MODE == SINGLE
        //TODO: different for each side
        emptyOneshot = false;
        reportEmpty = false;

        //save the previous layer to be able 
        //to recognize potential layer changes
        previousLayer = currentLayer;

        //read the currently active keys into their respective buffers
        updateBuffers(currentLayer);

        //reset the report along with the mods
        resetReport();

        //to make sure that the layer returns to 0
        //if no layer keys are being pressed
        resetLayer();

        //the toggle iterator starts at the reverse begin
        auto toggle_it = toggleBuffer.rbegin();

        for (int i = 1; i < 7; ++i)
        {

            if (momentaryBuffer.size() != 0)
            {
                //get the last element of the momentary buffer
                std::pair<uint8_t, uint8_t> rp = momentaryBuffer.back();

                //add it into the currentReport
                intoReport(rp.first, rp.second, i);

                //remove the pair to prevent it from being added to 
                //the buffer more than once
                momentaryBuffer.pop_back();
            }
            else if (toggleBuffer.size() != 0 && toggle_it != toggleBuffer.rend())
            {
                //add the toggle keycode into the report and iterate
                intoReport(toggle_it->first, toggle_it->second, i);
                ++toggle_it;
            }
            else if (oneshotBuffer.size() != 0)
            {
                if (emptyOneshot)
                {
                    //the last pair that should be add to the report
                    auto rp = oneshotBuffer.back();

                    //ad
                    intoReport(rp.first, rp.second, i);

                    //remove the pair from the oneshot buffer
                    oneshotBuffer.pop_back();
                }
            }

            //if none of the buffers contain anything, break out of the for loop
            else 
            {
                //the report is empty if no buffers contain anything at the first pass
                if (i == 1)
                {
                    reportEmpty = true;
                }
                //break;
            }

            if (currentReport[i] != previousReport[i] || currentMod != previousMod)
            {
                reportChanged = true;
            }
        }

        //save the current mod to detect 
        //changes in the next iteration
        previousMod = currentMod;

        currentReport[0] = currentMod;
        currentReport[7] = currentLayer;
#else
        resetReport();
        resetLayer();

        reportEmpty = false;

        previousLayer = currentLayer;

        updateBuffers(currentLayer);

        //the report is changed if the the previous and current reports
        //aren't the same size or they are different
        reportChanged = (previousReport.size() != currentReport.size()) ||
            !std::equal(std::begin(previousReport), std::end(previousReport),
                    std::begin(currentReport));

        reportEmpty = currentReport.size() == 0;
#endif 
    }

    void updateRemoteLayer(uint8_t layer)
    {
        //save the remoteLayer for comparison against
        //localLayer
        remoteLayer = layer;

        //the layer that is sent as remote layer should
        //always be the correct one because the remote's
        //currentLayer depends on the remote's remoteLayer,
        //so basically the localLayer here
        currentLayer = layer;
    }

#if KEYBOARD_MODE == HUB
    //only called on client 
    void updateRemoteReport(const std::vector<uint8_t>& report)
    {
        for (auto& code : report)
        {
            //decode the uint8_t into a keycode and add
            //it to the correct buffer
            intoBuffers(Keycode::decode(code));
        }
    }
#endif
}
