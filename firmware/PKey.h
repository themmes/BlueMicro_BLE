/*
Copyright 2019 <Julian Komaromy>

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

#include "VKey.h"
#include "keymap.h"
#include <array>
#include <utility>

#ifndef PKEY
#define PKEY

using VirtualKeys = std::array<VKey, NUM_LAYERS>;

class PKey 
{
    private:
        VirtualKeys vkeys;

        unsigned long lastChanged;
        bool wasPress;

        void update(unsigned long currentMillis, uint8_t layer, bool press)
        {
            auto delta = currentMillis - lastChanged;

            if (press)
            {
                vkeys[layer].press(delta, wasPress);
            }
            else 
            {
                vkeys[layer].clear(delta, wasPress);
            }

            updateLastChanged(currentMillis, press);
        }

        void updateLastChanged(unsigned long currentMillis, bool press)
        {
            if (wasPress != press)
            {
                lastChanged = currentMillis;
                
                //flip wasPress
                wasPress = press;
            }
        }
        //sets the keycode and duration for a given index and code
        /*
           void setInfo(std::size_t i, uint32_t activation) 
           {
        //set the keycode to the first 8 bits of the code 
        keycodes[i] = static_cast<uint16_t>(activation & 0x0000FFFF);

        //set the duration to the 9-14th bits of the code
        durations[i] = static_cast<uint8_t>((activation & 0x00FF0000) >> 16);
        };
        */

        //Keypress state;

        //Durations durations {{0}};
        //Keycodes keycodes {{0}};

    public:
        //default is no virtual keys whatsoever 
        PKey() : vkeys {{ }} { }

        //rvalue constructor
        PKey(VirtualKeys&& keys) : vkeys {std::move(keys)} { }

        //lvalue constructor
        PKey(const VirtualKeys& keys) : vkeys {keys} { }

        void press(unsigned long currentMillis, uint8_t layer) 
        { 
            update(currentMillis, layer, true);
        }

        void clear(unsigned long currentMillis, uint8_t layer) 
        { 
            update(currentMillis, layer, false);
        }

        /*
         * return the keycode if one of the keys is active
         */
        Keycode getKeycode(uint8_t layer) 
        {
            return vkeys[layer].getKeycode();
        }
};

#endif
