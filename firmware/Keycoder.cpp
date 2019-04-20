#include "Keycoder.h"

namespace Keycoder
{
    std::array<Keycode, 256> keycodes;

    //copy the keycodes into the keycode array
    void generateKeycodes(const matrix_t& matrix) 
    {
        std::set<uint8_t> buffer;
        for (auto& l : matrix)
        {
            for (auto& r : l)
            {
                for (auto& vk : r)
                {
                    auto current = vk.getKeycodes();
                    buffer.insert(current.begin(), current.end());
                }
            }
        }
    }

    uint8_t encode(Keycode kc)
    {
        //find the index of the keycode inside the keycode array
        //also cast it to a byte, this always has to succeed
        //because the array is of size 2^8
        return static_cast<uint8_t>(std::distance(keycodes.begin(), 
                    std::find(keycodes.begin(), keycodes.end(), kc))); 
    }

    Keycode decode(uint8_t kc)
    {
        return keycodes[kc];
    }
}
