
#include "VKey.h"
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
                vkeys[layer].press(delta);
            }
            else 
            {
                vkeys[layer].clear(delta);
            }

            updateLastChanged(currentMillis, press);
        }

        void updateLastChanged(unsigned long currentMillis, bool press)
        {
            if (wasPress != press)
            {
                lastChanged = currentMillis;
                wasPress = !wasPress;
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
        Key() : vkeys {{ }} { }

        //rvalue constructor
        Key(VirtualKeys&& keys) : vkeys {std::move(keys)} { }

        //lvalue constructor
        Key(const VirtualKey& keys) : vkeys {keys} { }

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
}

#endif
