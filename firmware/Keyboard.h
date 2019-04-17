#include <vector>

#include "keymap.h"
#include "PKey.h"
#include "VKey.h"

#include <algorithm>
#include <cstdint>
#include <utility>
#include <array>

#ifndef KEYBOARD
#define KEYBOARD

namespace Keyboard 
{
    void setupKeyboard();
    
    void scanMatrix(int currentState, unsigned long currentMillis, int row, int col);

    std::array<uint8_t, 8> getCurrentReport();

    void updateReport();

    bool getReportEmpty();

    bool getReportChanged();
    bool getLayerChanged();

    //also resets the layer changed flag
    uint8_t getLocalLayer();

    unsigned long getLastPressed();

    void updateRemoteLayer(uint8_t layer);
    void updateRemoteReport(std::array<uint8_t, 7> report);
};

#endif
