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
    {KEYMAP( KC_1,    KC_2,    KC_3, KC_4,    KC_5,    KC_6,  KC_7,    KC_8,    KC_9  )};

 
void setupKeymap() {
;

// Code below makes sure that the encoder gets configured.

  RotaryEncoder.begin(ENCODER_PAD_A, ENCODER_PAD_B);    // Initialize Encoder
  RotaryEncoder.setCallback(encoder_callback);    // Set callback
  RotaryEncoder.start();    // Start encoder

}

void encoder_callback(int step)
{
  keyboardstate.encoder1pos = keyboardstate.encoder1pos + step; 
  uint8_t layer = keyboardstate.layer;

  if (abs(keyboardstate.encoder1pos) > ENCODER_RESOLUTION)
  {
    if ( keyboardstate.encoder1pos < 0  )
    {
            switch(layer)
        {
            case _L0: KeyScanner::add_to_encoderKeys(KC_AUDIO_VOL_UP); break;
            case _L1: KeyScanner::add_to_encoderKeys(KC_RIGHT); break;
            case _L2: KeyScanner::add_to_encoderKeys(LSFT(KC_RIGHT)); break;
            default: ;
        }
    }
    else 
    {
        switch(layer)
        {
            case _L0: KeyScanner::add_to_encoderKeys(KC_AUDIO_VOL_DOWN); break;
            case _L1: KeyScanner::add_to_encoderKeys(KC_LEFT);break;
            case _L2: KeyScanner::add_to_encoderKeys(LSFT(KC_LEFT));break;
            default: ;
        }
    } 
    keyboardstate.encoder1pos = 0; 
  } 
}
