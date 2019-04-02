#include <utility>
#include <vector>
#include <array>

#ifndef KEYPRESS_H
#define KEYPRESS_H

using Method = std::vector<std::pair<bool, uint32_t>>;

class Keypress 
{
    public: 
        Keypress();
        Keypress(Method ms); 

        void clear(unsigned long delta, bool wasPress);
        void press(unsigned long delta, bool wasPress);

        bool isActive();
    private:
        void checkActivations(unsigned long delta, bool wasPress, bool press);

        bool extrema(unsigned long delta, 
                std::pair<bool, uint8_t> current, 
                bool wasPress,
                bool press, 
                bool even);

        void updateIndex();

        Method method;

        bool wasPress;

        std::size_t index;
};

#endif 
