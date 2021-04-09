/*
Copyright 2018 <Pierre Constantineau>

3-Clause BSD License

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#include "keymap.h"

std::array<std::array<Key, MATRIX_COLS>, MATRIX_ROWS> matrix =
        KEYMAP2ARRAY(KEYMAP(
    KC_NO,    KC_NO,    KC_NO,    KC_NO,    
    KC_NO,    KC_NO,    KC_NO,    KC_NO,  
    KC_NO,    KC_NO,    KC_NO,    KC_NO,    
    KC_NO,    KC_NO,    KC_NO,    KC_NO
        ));


 
void setupKeymap() {
    setupKeymapArrows();
}


void setupKeymapArrows() {

    uint32_t thislayer[MATRIX_ROWS][MATRIX_COLS] =
    KEYMAP(
    KC_HOME,    KC_UP,    KC_PGUP,    KC_ESCAPE,    
    KC_LEFT,    KC_NO,    KC_RIGHT,    KC_NO,  
    KC_END,     KC_DOWN,  KC_PGDN,    KC_KP_ENTER,    
    LAYER_1,    KC_INS,   KC_DEL,  KC_KP_ENTER);

    uint32_t layer1[MATRIX_ROWS][MATRIX_COLS] =
    KEYMAP(
    KMNUMPAD,   KMMACROS,   KMARROWS,       KMSHORTCUTS,    
    KC_NO,      KC_NO,      KC_NO,          KC_NO,  
    KC_NO,      KC_NO,      KC_NO,          KC_NO,    
    LAYER_1,    KC_NO,      PRINT_INFO,     PRINT_BATTERY);
    /*
     * add the other layers
     */
    for (int row = 0; row < MATRIX_ROWS; ++row)
    {
        for (int col = 0; col < MATRIX_COLS; ++col)
        {
            matrix[row][col].addActivation(_L0, Method::PRESS, thislayer[row][col]);
            matrix[row][col].addActivation(_L1, Method::PRESS, layer1[row][col]);

        }
    }
}

void setupKeymapNumpad() {

    uint32_t thislayer[MATRIX_ROWS][MATRIX_COLS] =
    KEYMAP(
        KC_7,    KC_8,    KC_9,    KC_KP_SLASH,
        KC_4,    KC_5,    KC_6,    KC_KP_ASTERISK,
        KC_1,    KC_2,    KC_3,    KC_KP_MINUS,
        LAYER_1, KC_0,    KC_DOT,  KC_KP_PLUS
    );

    uint32_t layer1[MATRIX_ROWS][MATRIX_COLS] =
    KEYMAP(
    KMNUMPAD,   KMMACROS,   KMARROWS,       KMSHORTCUTS,    
    KC_NO,      KC_NO,      KC_NO,          KC_NO,  
    KC_NO,      KC_NO,      KC_NO,          KC_NO,    
    LAYER_1,    KC_NO,      PRINT_INFO,     PRINT_BATTERY);
    /*
     * add the other layers
     */
    for (int row = 0; row < MATRIX_ROWS; ++row)
    {
        for (int col = 0; col < MATRIX_COLS; ++col)
        {
            matrix[row][col].addActivation(_L0, Method::PRESS, thislayer[row][col]);
            matrix[row][col].addActivation(_L1, Method::PRESS, layer1[row][col]);

        }
    }
}

void setupKeymapMacros() {

    uint32_t thislayer[MATRIX_ROWS][MATRIX_COLS] =
    KEYMAP(
        CBR_FN,    IPADDR,         KC_NO,      GITCOMMIT,
        BRC_FN,    KC_NO,          KC_NO,      KC_NO,
        PRN_FN,    KC_NO,          KC_NO,      KC_NO,
        LAYER_1,     KC_NO,          KC_NO,      SMILE
    );

    uint32_t layer1[MATRIX_ROWS][MATRIX_COLS] =
    KEYMAP(
    KMNUMPAD,   KMMACROS,   KMARROWS,       KMSHORTCUTS,    
    KC_NO,      KC_NO,      KC_NO,          KC_NO,  
    KC_NO,      KC_NO,      KC_NO,          KC_NO,    
    LAYER_1,    KC_NO,      PRINT_INFO,     PRINT_BATTERY);
    /*
     * add the other layers
     */
    for (int row = 0; row < MATRIX_ROWS; ++row)
    {
        for (int col = 0; col < MATRIX_COLS; ++col)
        {
            matrix[row][col].addActivation(_L0, Method::PRESS, thislayer[row][col]);
            matrix[row][col].addActivation(_L1, Method::PRESS, layer1[row][col]);

        }
    }
}


void setupKeymapShortcuts() {

    uint32_t thislayer[MATRIX_ROWS][MATRIX_COLS] =
    KEYMAP(
        LGUI(KC_1),    LGUI(KC_2),    LGUI(KC_3) ,    LGUI(KC_4),
        LGUI(KC_5),    LGUI(KC_6),    LGUI(KC_7) ,    LGUI(KC_8),
        KC_NO,         KC_NO,          KC_NO,      KC_NO,
        LAYER_1,       KC_NO,          KC_NO,      KC_NO
    );

    uint32_t layer1[MATRIX_ROWS][MATRIX_COLS] =
    KEYMAP(
    KMNUMPAD,   KMMACROS,   KMARROWS,       KMSHORTCUTS,    
    KC_NO,      KC_NO,      KC_NO,          KC_NO,  
    KC_NO,      KC_NO,      KC_NO,          KC_NO,    
    LAYER_1,    KC_NO,      PRINT_INFO,     PRINT_BATTERY);
    /*
     * add the other layers
     */
    for (int row = 0; row < MATRIX_ROWS; ++row)
    {
        for (int col = 0; col < MATRIX_COLS; ++col)
        {
            matrix[row][col].addActivation(_L0, Method::PRESS, thislayer[row][col]);
            matrix[row][col].addActivation(_L1, Method::PRESS, layer1[row][col]);

        }
    }
}

void process_user_macros(uint16_t macroid)
{
    switch ((macroid))
    { 
        case GITCOMMIT:
            addStringToQueue("git add .");
            addKeycodeToQueue(KC_ENTER);
            addStringToQueue("git commit -m \"\""); 
            addKeycodeToQueue(KC_LEFT);
        break;
        case (CBR_FN):
            addStringToQueue("{}"); 
            addKeycodeToQueue(KC_LEFT);
        break;
        case (BRC_FN):
            addStringToQueue("[]");
            addKeycodeToQueue(KC_LEFT);
        break;
        case PRN_FN:
            addStringToQueue("()"); 
            addKeycodeToQueue(KC_LEFT);
        break;
        case IPADDR:
            addStringToQueue("192.168.1.");
        break;
        case SMILE:
            addStringToQueue(":)");
        break;
        case KMNUMPAD:
            setupKeymapNumpad();
        break;
        case KMMACROS:
            setupKeymapMacros();
        break;
        case KMARROWS:
            setupKeymapArrows();
        break;
        case KMSHORTCUTS:
            setupKeymapShortcuts();
        break;
    }
}