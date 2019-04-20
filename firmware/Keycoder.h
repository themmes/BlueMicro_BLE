#include "Keycode.h"
#include "keymap.h"
#include <algorithm>
#include <set>

#ifndef KEYCODER
#define KEYCODER

namespace Keycoder
{
    void generateKeycodes(const matrix_t& matrix);

    uint8_t encode(Keycode kc);
    Keycode decode(uint8_t kc);
};

#endif 
