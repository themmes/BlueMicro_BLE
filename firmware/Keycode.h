#include <cstdint>

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
            duration = static_cast<uint8_t>((keycode & 0x0000FF00) >> 8);
            modifiers = static_cast<uint8_t>((keycode & 0x00FF0000) >> 16);
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
};

#endif 
