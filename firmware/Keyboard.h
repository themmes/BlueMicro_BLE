#include "keymap.h"
#include "PKey.h"
#include "VKey.h"
#include "Keycoder.h"

#include <algorithm>
#include <cstdint>
#include <utility>
#include <array>
#include <vector>

#ifndef KEYBOARD
#define KEYBOARD

namespace Keyboard 
{
    void setupKeyboard();
    
    void scanMatrix(int currentState, unsigned long currentMillis, int row, int col);

#if KEYBOARD_MODE == HUB || KEYBOARD_MODE == SINGLE
    std::array<uint8_t, 8> getCurrentReport();
#else
    std::vector<uint8_t> getCurrentReport();
#endif

    void updateReport();

    bool getReportEmpty();

    bool getReportChanged();
    bool getLayerChanged();

    //also resets the layer changed flag
    uint8_t getLocalLayer();

    unsigned long getLastPressed();

    void updateRemoteLayer(uint8_t layer);

#if KEYBOARD_MODE == HUB
    void updateRemoteReport(std::vector<uint8_t> report);
#endif
};

#endif
