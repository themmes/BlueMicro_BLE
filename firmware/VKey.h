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

#include "Keycode.h"
#include "Keypress.h"
#include <array>
#include <utility>
#include <memory>
#include <set>

#ifndef VKEY
#define VKEY

#define DEBOUNCE_TIME 1 //1*10ms = 10ms debounce time by default

using kp_t = std::shared_ptr<Keypress>;
using act_t = std::pair<Keycode, kp_t>;
using activations_t = std::array<act_t, 3>;

//a thin wrapper around the activations
//the class has no notion of layers or the delta
class VKey
{
    private: 
        activations_t activations;

    public:
        VKey() : activations {{ }} {}

        /*
        VKey(uint32_t keycode) 
        { 
            //initialize the default keypress:
            //press for minimum of debounce time
            Keypress default_kp {{ { false, DEBOUNCE_TIME } }};
            activations = {{ {keycode, std::make_shared<Keypress>(default_kp)} }};
        }
        */
      
        //initialize the default keypress:
        //press for minimum of debounce time
        VKey(uint32_t keycode) : activations {{ { keycode, 
            std::make_shared<Keypress>(Keypress {{ {false, DEBOUNCE_TIME} }}) } }} { }

        VKey(Keypress kp, uint32_t kc) : activations {{ { kc,
            std::make_shared<Keypress>(kp) } }} { }

        //by default, keypresses are nullptrs to avoid
        //unnecessary allocation and keycodes are 0
        VKey(kp_t k1, uint32_t a1, 
                kp_t k2 = nullptr, uint32_t a2 = 0, 
                kp_t k3 = nullptr, uint32_t a3 = 0)
            : activations {{ {a1, k1}, {a2, k2}, {a3, k3} }} { }

        void press(unsigned long delta, bool wasPress)
        {
            //loop through the activations, avoiding nullptrs
            for (auto& p : activations)
            {
                if (p.second.get() != nullptr)
                {
                    p.second->press(delta, wasPress);
                }
            }
        }

        void clear(unsigned long delta, bool wasPress)
        {
            //loop through the activations, avoiding nullptrs
            for (auto& p : activations)
            {
                if (p.second.get() != nullptr)
                {
                    p.second->clear(delta, wasPress);
                }
            }
        }

        Keycode getKeycode()
        {
            //loop through the activations, avoiding nullptrs
            for (auto& p : activations)
            {
                if (p.second.get() != nullptr)
                {
                    if (p.second->isActive())
                    {
                        return p.first;
                    }
                }   
            }

            return {};
        }

        std::set<Keycode> getKeycodes() const
        {
            std::set<Keycode> ret; 

            for (const auto& p : activations)
            {
                if (p.second.get() != nullptr)
                {
                    ret.insert(p.first);
                }   
            }

            return ret;
        }
};

#endif
