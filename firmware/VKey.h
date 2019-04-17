#include "Keycode.h"
#include "Keypress.h"
#include <array>
#include <utility>
#include <memory>

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
};

#endif
