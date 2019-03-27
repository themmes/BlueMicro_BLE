#include "Keypress.h"

namespace std {
    void __throw_bad_alloc() {
    }
    void __throw_length_error( char const*e ) {
    }
}

Keypress::Keypress()
{
    activations = {{ }};
    candidates = {0};

    lastChange = 0;
    wasPress = false;
}

Keypress::Keypress(Activations acts) 
{
    activations = acts;
    candidates = {0};

    lastChange = 0;
    wasPress = false;
} 

void Keypress::clear(unsigned long currentMillis) 
{
    checkActivations(currentMillis, false);
}

void Keypress::press(unsigned long currentMillis) 
{
    checkActivations(currentMillis, true);
}

//read all of the activations and give back true
//in the respective index of the array if it is activated
std::array<bool, 3> Keypress::read() 
{
    //nothing is active by default
    std::array<bool, 3> ret = {false, false, false};

    //loop though the activations
    for (auto i = 0; i < activations.size(); ++i) 
    {
        //avoid empty activation methods
        if (activations[i].size() != 0)
        {
            //if the progress index shows that each
            //element of the activation method has been
            //completed
            if (candidates[i] == activations[i].size()) 
            {
                //give an active reading and reset the index
                ret[i] = true;
                candidates[i] = 0;
            }
        }
    }

    return ret;
}

//returns a failure value (4) or the index of the first active activation
std::size_t Keypress::singleRead()
{
    for (auto i = 0; i < activations.size(); ++i) 
    {
        if (activations[i].size() != 0)
        {
            if (candidates[i] == activations[i].size())
            {
                //reset the index and immediately return, leaving
                //the other candidates unchanged
                candidates[i] = 0;
                return i;
            }
        }
    }

    //since the maximum amount of activations is 3, 
    //4 represents nothing being active
    return 4;
}

void Keypress::updateCandidate(std::size_t i) {

    if (candidates[i] != activations[i].size()) 
    {
        candidates[i]++;
    }
}

bool Keypress::extrema(uint32_t delta, 
        std::pair<bool, uint32_t> current, 
        bool press, 
        bool even) {

    bool max = current.first;

    if (( press && (even != max)) ||
            (!press && (even == max)))
    {
        if (((delta <= current.second*10) == max)
                && (wasPress == even))
        {
            return true; 
        }
    }

    return false;
}

void Keypress::updateLastOn(unsigned long currentMillis, bool press)
{
    if (wasPress != press) 
    {
        lastChange = currentMillis;
        wasPress = !wasPress;
    }
}

void Keypress::checkActivations(unsigned long currentMillis, bool press) 
{
    //Calculate the elapsed time since the last change of activations
    unsigned long delta = currentMillis - lastChange;

    //Loop throught the activations
    for (std::size_t i = 0; i < activations.size(); ++i) 
    {
        //Avoid empty, unused activation methods
        if (activations[i].size() != 0) 
        {
            //Retrieve the next action that should be taken to progress
            //the activation method
            auto current = activations[i][candidates[i]];

            //check if the current action brings the activation
            //method forwards
            if (extrema(delta, current, press, (candidates[i] % 2 == 0)))
            {
                //iterate the candidate
                updateCandidate(i);
            }
        }
    }

    updateLastOn(currentMillis, press);
}
