#include "Keycode.h"

Keycode::Keycode() : hid_keycode{0}, modifiers{0}, duration{0} { }

Keycode::Keycode(uint32_t keycode)
{
    //initialize the three different keycode components
    hid_keycode = static_cast<uint8_t>(keycode & 0x000000FF);
    modifiers = static_cast<uint8_t>((keycode & 0x0000FF00) >> 8);
    duration = static_cast<uint8_t>((keycode & 0x00FF0000) >> 16);
    
    //if this activation isn't yet in the keycode array, add it
    if (std::find(std::begin(keycodes), std::end(keycodes), *this)
            == std::end(keycodes))
    {
        keycodes[index] = *this;
        ++index;
    }

}

//the keycode is relevant if it has 
//a non null hid keycode or some extra modifiers
bool Keycode::isRelevant() const 
{
    return hid_keycode != 0 || modifiers != 0;
}

/* 
 * basic get functions
 */
uint8_t Keycode::getModifiers() const { return modifiers; }
uint8_t Keycode::getHIDKeycode() const { return hid_keycode; }
uint8_t Keycode::getDuration() const { return duration; }

std::pair<uint8_t, uint8_t> Keycode::getReportPair() const
{
    return std::make_pair(hid_keycode, modifiers);
}

//allow implicit conversion to a uint32_t
Keycode::operator uint32_t() const 
{
    return (static_cast<uint32_t>(hid_keycode) 
            | (static_cast<uint32_t>(modifiers) << 8)
            | (static_cast<uint32_t>(duration) << 16));
}

//encode the Keycode into a uint8_t
//for sending across bluetooth
uint8_t Keycode::encode() const
{
    //find the keycode in the keycodes array and return its index
    return static_cast<uint8_t>(std::distance(std::begin(keycodes), 
                std::find(std::begin(keycodes), std::end(keycodes), *this)));
}

//decode a uint8_t received from bluetooth
Keycode Keycode::decode(uint8_t kc)
{
    return keycodes[kc];
}

/*
 * an array of all keycodes, with each index
 * corresponding to a uint8_t that can be sent
 * across bluetooth
 */
std::array<Keycode, 256> Keycode::keycodes { };

//the current index of the above array
uint8_t Keycode::index {0};
