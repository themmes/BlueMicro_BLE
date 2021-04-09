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

        uint32_t press[MATRIX_ROWS][MATRIX_COLS] = KEYMAP(
        KC_NO,    KC_8,    KC_9,   KC_NO,
        KC_4,    KC_5,    KC_6,    KC_NO,
        KC_1,    KC_2,    KC_3,    KC_KP_MINUS,
        KC_0,    KC_0,    KC_DOT,  KC_KP_PLUS 
        );


    uint32_t tap[MATRIX_ROWS][MATRIX_COLS] =
        KEYMAP(
    KC_LBRACKET,    KC_NO,    KC_NO,    KC_RBRACKET,    
    KC_NO,    KC_NO,    KC_NO,    KC_NO,  
    KC_NO,    KC_NO,    KC_NO,    KC_NO,    
    KC_NO,    KC_NO,    KC_NO,    KC_NO);

    uint32_t hold[MATRIX_ROWS][MATRIX_COLS] =
        KEYMAP(
    KC_LCBR,    KC_NO,    KC_NO,    KC_RCBR,    
    KC_NO,    KC_NO,    KC_NO,    KC_NO,  
    KC_NO,    KC_NO,    KC_NO,    KC_NO,    
    KC_NO,    KC_NO,    KC_NO,    KC_NO);

    uint32_t singletap[MATRIX_ROWS][MATRIX_COLS] =
        KEYMAP(
    KC_NO,    KC_NO,    KC_NO,    KC_NO,    
    KC_NO,    KC_NO,    KC_NO,    KC_QUOTE,  
    KC_NO,    KC_NO,    KC_NO,    KC_NO,    
    KC_NO,    KC_NO,    KC_NO,    KC_NO);

    uint32_t doubletap[MATRIX_ROWS][MATRIX_COLS] =
        KEYMAP(
    KC_NO,    KC_NO,    KC_NO,    KC_NO,    
    KC_NO,    KC_NO,    KC_NO,    KC_DQUO,  
    KC_NO,    KC_NO,    KC_NO,    KC_NO,    
    KC_NO,    KC_NO,    KC_NO,    KC_NO);

    /*
     * add the other layers
     */
    for (int row = 0; row < MATRIX_ROWS; ++row)
    {
        for (int col = 0; col < MATRIX_COLS; ++col)
        {
            //Method::PRESS  Standard keypress
            //Method::MT_TAP  This key is sent once when the key is tapped.
            //Method::MT_HOLD This key is sent once when the key is held.
            //Method::DT_TAP  This key is sent once when the key is tapped.
            //Method::DT_DOUBLETAP This key is sent once when the key is tapped twice.
            matrix[row][col].addActivation(_L0, Method::PRESS, press[row][col]);
            matrix[row][col].addActivation(_L0, Method::MT_TAP, tap[row][col]);
            matrix[row][col].addActivation(_L0, Method::MT_HOLD, hold[row][col]);
            matrix[row][col].addActivation(_L0, Method::DT_TAP,  singletap[row][col]);
            matrix[row][col].addActivation(_L0, Method::DT_DOUBLETAP, doubletap[row][col]);
        }
    }

}

