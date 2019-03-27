/*
Copyright 2018 <Julian Komaromy>

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
#include "Keypress.h"
#include "hid_keycodes.h"
#include <array>
#include <utility>

#ifndef KEY_H
#define KEY_H

using Durations = std::array<uint8_t, 3>;
using Keycodes = std::array<uint16_t, 3>;
using Method = std::vector<std::pair<bool, uint32_t>>;

class Key 
{
    private:

        //sets the keycode and duration for a given index and code
        void setInfo(std::size_t i, uint32_t activation) 
        {
            keycodes = {0, 0, 0};
            //set the keycode to the first 8 bits of the code 
            keycodes[i] = static_cast<uint16_t>(activation & 0x0000FFFF);

            //set the duration to the 9-14th bits of the code
            durations[i] = static_cast<uint16_t>((activation & 0x00FF0000) >> 16);
        };

        Keypress state;

        Durations durations;
        Keycodes keycodes;

    public:
        Key() 
        {
            state = Keypress {{ }};
            durations = {0, 0, 0}; 
        }

        Key(uint32_t activation) 
        {
            durations = {0, 0, 0}; 

            //default activation: press for minimum 0 ms
            state = Keypress {{ {{ {false, 0} }} }}; 

            //set the keycode and activation
            setInfo(0, activation);
        }

        Key(Method m1, uint32_t a1)
        {
            durations = {0, 0, 0}; 
            state = Keypress {{ m1 }};
            setInfo(0, a1);
        }

        Key(Method m1, uint32_t a1, Method m2, uint32_t a2)
        {
            durations = {0, 0, 0}; 
            state = Keypress {{ m1, m2 }};
            setInfo(0, a1);
            setInfo(0, a2);
        }

        Key(Method m1, uint32_t a1, Method m2, uint32_t a2, Method m3, uint32_t a3)
        {
            durations = {0, 0, 0}; 
            state = Keypress {{ m1, m2, m3 }};
            setInfo(0, a1);
            setInfo(0, a2);
            setInfo(0, a3);
        }


        void press(unsigned long currentMillis) { state.press(currentMillis); }
        void clear(unsigned long currentMillis) { state.press(currentMillis); }

        /*
         * return a pair of the keycode and activation duration
         * TODO: Try compilation for C++17, if it works, use std::optional
         */
        std::pair<uint16_t, uint8_t> getPair() 
        {
            auto reading = state.singleRead();

            //4 represents nothing being active
            if (reading == 4) 
            {
                //return a zero pair
                return std::make_pair(0, 0); 
            }

            return std::make_pair(keycodes[reading], durations[reading]);
        }
};
#endif /* KEY_H */
