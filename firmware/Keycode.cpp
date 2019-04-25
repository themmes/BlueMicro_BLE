#include "Keycode.h"

Keycode::Keycode() : hid_keycode{0}, modifiers{0}, duration{0} { }

Keycode::Keycode(uint32_t keycode)
{
    hid_keycode = static_cast<uint8_t>(keycode & 0x000000FF);
    modifiers = static_cast<uint8_t>((keycode & 0x0000FF00) >> 8);
    duration = static_cast<uint8_t>((keycode & 0x00FF0000) >> 16);
    
    //if this activation isn't yet in the array, add it
    if (std::find(std::begin(keycodes), std::end(keycodes), *this)
            == std::end(keycodes))
    {
        keycodes[index] = *this;
        ++index;
    }

}


uint8_t Keycode::getModifiers() const
{
    return modifiers;
}

uint8_t Keycode::getHIDKeycode() const
{
    return hid_keycode;
}

uint8_t Keycode::getDuration() const
{
    return duration;
}

Keycode::operator uint32_t() const 
{
    return (static_cast<uint32_t>(hid_keycode) 
            | (static_cast<uint32_t>(modifiers) << 8)
            | (static_cast<uint32_t>(duration) << 16));
}

bool operator<(const Keycode& l, const Keycode& r)
{
    return std::tie(l.hid_keycode, l.modifiers, l.duration) <
        std::tie(r.hid_keycode, r.modifiers, r.duration);
}

bool operator==(const Keycode& l, const Keycode& r)
{
    return std::tie(l.hid_keycode, l.modifiers, l.duration) == 
        std::tie(r.hid_keycode, r.modifiers, r.duration);
}

uint8_t Keycode::encode() const
{
    return static_cast<uint8_t>(std::distance(std::begin(keycodes), 
                std::find(std::begin(keycodes), std::end(keycodes), *this)));
}

Keycode Keycode::decode(uint8_t kc)
{
    return keycodes[kc];
}

std::array<Keycode, 256> Keycode::keycodes { };
uint8_t Keycode::index {0};
