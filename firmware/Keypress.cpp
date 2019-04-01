#include "Keypress.h"

namespace std {
    void __throw_bad_alloc() {
    }
    void __throw_length_error( char const*e ) {
    }
}

Keypress::Keypress()
{
    method{};
    wasPress = false;
}

Keypress::Keypress(Methods ms) 
{
    activations = ms;
    candidates = {0};

    lastChange = 0;
    wasPress = false;
} 

void Keypress::clear(unsigned long delta, bool wasPress) 
{
    checkActivations(delta, wasPress, false);
}

void Keypress::press(unsigned long delta, bool wasPress) 
{
    checkActivations(delta, delta, true);
}

//returns a failure value (4) or the index of the first active activation
bool Keypress::isActive()
{
    if (method.size() != 0 && index == method.size())
    {
        //reset the index and return
        candidates[i] = 0;
        return i;
    }

    //the method is otherwise not active yet
    return false;
}

void Keypress::updateIndex()
{
    if (index != method.size()) 
    {
        index++;
    }
}

bool Keypress::extrema(unsigned long delta, 
        std::pair<bool, uint8_t> current, 
        bool wasPress,
        bool press, 
        bool even) {

    bool max = current.first;
    
    //TODO: Could put ifs into return...
    if ((press && (even != max)) ||
            (!press && (even == max)))
    {
        //cast to make sure comparison succeeds, unsure if this is needed
        if (((delta <= static_cast<unsigned long>(current.second*10)) == max)
                && (wasPress == even))
        {
            return true; 
        }
    }

    return false;
}

void Keypress::checkActivations(unsigned long delta, bool wasPress, bool press) 
{
    if (method.size() != 0) 
    {
        //Retrieve the next action that should be taken to progress the method
        auto current = method[index];

        //check if the current action brings the 
        //activation method forwards
        if (extrema(delta, current, wasPress, press, (index % 2 == 0)))
        {
            //iterate the index
            updateIndex(i);
        }
    }
}
