/*
Copyright 2018 <Pierre Constantineau>

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
#ifndef KEYBOARD_CONFIG_H
#define KEYBOARD_CONFIG_H
#include "hardware_config.h"

#define KEYBOARD_SIDE SINGLE

#define DEVICE_NAME_M                         "PhoneKeyboard"                          /**< Name of device. Will be included in the advertising data. */

#define DEVICE_MODEL                        "PhoneKeyboard_v1"                          /**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME                   "jpconstantineau.com"                      /**< Manufacturer. Will be passed to Device Information Service. */


#define KEYMAP( \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K010, K011, K012, K013, K014, \
    K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K110, K111, K112, K113, K114, \
    K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K210, K211, K212, K213, K214, \
    K30, K31, K32, K33, K34, K35, K36, K37, K38, K39, K310, K311, K312, K313, K314, \
    K40, K41, K42, K43, K44, K45, K46, K47, K48, K49, K410, K411, K412, K413, K414\
) { \
    { K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K010, K011, K012, K013, K014 }, \
    { K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K110, K111, K112, K113, K114 }, \
    { K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K210, K211, K212, K213, K214 }, \
    { K30, K31, K32, K33, K34, K35, K36, K37, K38, K39, K310, K311, K312, K313, K314 }, \
    { K40, K41, K42, K43, K44, K45, K46, K47, K48, K49, K410, K411, K412, K413, K414 } \
}

#endif /* KEYBOARD_CONFIG_H */
