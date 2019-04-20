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

#ifndef KEYCODE
#define KEYCODE

class Keycode 
{
    private:
        uint8_t hid_keycode;
        uint8_t modifiers;
        uint8_t duration;
    public:
        Keycode(): hid_keycode{0}, modifiers{0}, duration{0} { }

        Keycode(uint32_t keycode)
        {
            hid_keycode = static_cast<uint8_t>(keycode & 0x000000FF);
            modifiers = static_cast<uint8_t>((keycode & 0x0000FF00) >> 8);
            duration = static_cast<uint8_t>((keycode & 0x00FF0000) >> 16);
        }

        uint8_t getModifiers() 
        {
            return modifiers;
        }

        uint8_t getHIDKeycode()
        {
            return hid_keycode;
        }

        uint8_t getDuration()
        {
            return duration;
        }

        //allows for implicit conversion to a 
        //uint32_t 
        operator uint32_t() const 
        {
            return (static_cast<uint32_t>(hid_keycode) 
                    | (static_cast<uint32_t>(modifiers) << 8)
                    | (static_cast<uint32_t>(duration) << 16));
        }


        friend bool operator<(const Keycode& l, const Keycode& r)
        {
            return std::tie(l.hid_keycode, l.modifiers, l.duration) <
                std::tie(r.hid_keycode, r.modifiers, r.duration);
        }

        friend bool operator==(const Keycode& l, const Keycode& r)
        {
            return std::tie(l.hid_keycode, l.modifiers, l.duration) == 
                std::tie(r.hid_keycode, r.modifiers, r.duration);
        }
};

#endif 
