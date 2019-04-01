#include <cstdint>
#include <utility>
#include <array>
#include <vector>

#include "keymap.h"
#include "PKey.h"
#include "VKey.h"

#ifndef KEYBOARD
#define KEYBOARD

namespace Keyboard 
{
    void setupKeyboard();
    
    void scanMatrix(int currentState, unsigned long currentMillis, int row, int col);

    std::array<uint8_t, 8> getCurrentReport();

    //bool getReport(); return value isn't used, calls layer, mod update and updates buffer
    void updateReport();

    bool reportEmpty();
    bool layerChanged();

    //also resets the layer changed flag
    uint8_t getLocalLayer();

    unsigned long getLastPressed();

    void updateRemoteLayer(uint8_t layer);
    void updateRemoteReport(std::array<uint8_t, 7> report);
};

#endif
