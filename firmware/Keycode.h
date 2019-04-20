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
