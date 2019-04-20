/*
Copyright 2018 <Pierre Constantineau, Julian Komaromy>

3-Clause BSD License

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef FIRMWARE_CONFIG_H
#define FIRMWARE_CONFIG_H

#define HUB 0
#define SPOKE 1
#define SINGLE 2

#include "avr_mapping.h"
#include "keyboard_config.h"

//the matrix should be pretty much always
//scanned as well as the keys sent
#define MATRIX_SCAN 1
#define SEND_KEYS 1

//default values for the LEFT side of the keyboard
#if KEYBOARD_MODE == HUB
#define BLE_PERIPHERAL_COUNT 1
#define BLE_HID_COUNT 1
#define BLE_CENTRAL_COUNT 1
#define DEVICE_NAME DEVICE_NAME_L
#define KBLINK_CLIENT 

//default values for the RIGHT side of the keyboard
#elif KEYBOARD_MODE == SPOKE
#define BLE_PERIPHERAL_COUNT 1
#define BLE_HID_COUNT 0
#define BLE_CENTRAL_COUNT 0
#define DEVICE_NAME DEVICE_NAME_R
#define KBLINK_SERVER

//default values for a SINGLE keyboard (a non-split keyboard)
#elif KEYBOARD_MODE == SINGLE
#define BLE_PERIPHERAL_COUNT 1
#define BLE_HID_COUNT 1
#define BLE_CENTRAL_COUNT 0
#define DEVICE_NAME DEVICE_NAME_S

//default values for a TEST keyboard (might be nonsensical)
#elif KEYBOARD_MODE == TEST
#define BLE_CENTRAL_COUNT 0
#define BLE_PERIPHERAL_COUNT 0 
#define BLE_HID_COUNT 1 
#define DEVICE_NAME DEVICE_NAME_S

//one of the previous values has to be defined
#else
#pragma GCC error "Keyboard side is neither LEFT or RIGHT nor MASTER"
#endif

#ifndef BOOT_MODE_COMMANDS
#define BOOT_MODE_COMMANDS {{KC_SPACE, STATE_MONITOR_MODE},{ KC_B,  STATE_BOOT_CLEAR_BONDS },{ KC_F, STATE_BOOT_SERIAL_DFU},{ KC_W, STATE_BOOT_WIRELESS_DFU}}
#define BOOT_MODE_COMMANDS_COUNT 4
#endif

#ifndef BOOT_MODE_DELAY
#define BOOT_MODE_DELAY  500
#endif

/*
 * now defined in VKey.h
#ifndef DEBOUNCETIME 
#define DEBOUNCETIME 10
#endif
*/

#ifndef HIDREPORTINGINTERVAL
#define HIDREPORTINGINTERVAL 1
#endif

// Battery Service definitions.

#ifndef BLE_LIPO_MONITORING
#define BLE_LIPO_MONITORING 0
#endif

#define SLEEPING_DELAY 30000              // when it's not connected, 30 seconds is good.
#define SLEEPING_DELAY_CONNECTED 600000   // 2 minutes is way too fast and really ennoying. making it 10 minutes
#define SLEEP_ACTIVE 0                    // 1 = it will go to sleep. 0 = sleep will not be activated.

#define DEFAULT_PWM_VALUE 1500            // PWM intensity
#define PWM_TOUCH_INTERVAL 1000           // detection time since last keypress.

#define VBAT_PIN          (A7)
#define VBAT_MV_PER_LSB   (0.73242188F)   // 3.0V ADC range and 12-bit ADC resolution = 3000mV/4096
#define VBAT_DIVIDER      (0.71275837F)   // 2M + 0.806M voltage divider on VBAT = (2M / (0.806M + 2M))
#define VBAT_DIVIDER_COMP (1.403F)        // Compensation factor for the VBAT divider

#endif /* FIRMWARE_CONFIG_H */
