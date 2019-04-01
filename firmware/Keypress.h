#include <utility>
#include <vector>
#include <array>

#ifndef KEYPRESS_H
#define KEYPRESS_H

using Modifier = std::vector<std::pair<bool, uint32_t>>;
using Methods = std::array<Modifier, 3>;

class Keypress 
{
    public: 
        Keypress();
        Keypress(Methods ms); 

        void clear(unsigned long currentMillis);
        void press(unsigned long currentMillis);

        //unused
        std::array<bool, 3> read();

        std::size_t singleRead();

    private:
        void checkActivations(unsigned long currentMillis, bool press);

        bool extrema(uint32_t delta, 
                std::pair<bool, uint32_t> current, 
                bool press, 
                bool even);

        void updateCandidate(std::size_t i);

        void updateLastOn(unsigned long currentMillis, bool press);

        Methods activations;

        bool wasPress;
        unsigned long lastChange;
        std::array<std::size_t, 3> candidates;
};

#endif 
