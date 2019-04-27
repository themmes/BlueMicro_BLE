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

#include <cstdint>
#include <tuple>
#include <array>
#include <algorithm>

#ifndef KEYCODE
#define KEYCODE

/*
 * a wrapper around the hid keycodes, the extra modifiers and the duration
 * of a keycode
 *
 * also contains static logic for coding a Keycode to
 * and from a uint8_t
 */
class Keycode 
{
    private:
        uint8_t hid_keycode;
        uint8_t modifiers;
        uint8_t duration;

        static std::array<Keycode, 256> keycodes;
        static uint8_t index;

    public:
        Keycode();

        Keycode(uint32_t keycode);

        uint8_t getModifiers() const;
        uint8_t getHIDKeycode() const;
        uint8_t getDuration() const;

        //returns a pair of the hid keycode and extra modifiers
        std::pair<uint8_t, uint8_t> getReportPair() const;

        //whether the keycode should be handled or not
        bool isRelevant() const;

        //allows for implicit conversion to a 
        //uint32_t 
        operator uint32_t() const;

        //encode a Keycode into a uin8_t for 
        //sending across bluetooth
        uint8_t encode() const;

        //decode a uint8_t received from bluetooth
        //into a full Keycode
        static Keycode decode(uint8_t kc);
};

#endif 
