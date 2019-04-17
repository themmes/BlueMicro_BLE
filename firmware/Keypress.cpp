#include "Keypress.h"

//avoid a compile time error
namespace std 
{
    void __throw_bad_alloc() { }
    void __throw_length_error( char const*e ) { }
}

Keypress::Keypress() : method{ }, activationIndex{1} { }

Keypress::Keypress(Method m) : method{m}, activationIndex{m.size()} { }

Keypress::Keypress(Method m, uint8_t actIndex) 
    : method{m}, activationIndex{actIndex} { }

void Keypress::clear(unsigned long delta, bool wasPress) 
{
    checkActivations(delta, wasPress, false);
}

void Keypress::press(unsigned long delta, bool wasPress) 
{
    checkActivations(delta, wasPress, true);
}

//returns a failure value (4) or the index of the first active activation
bool Keypress::isActive()
{
    //if the keypress is seen as active,
    //reset the active flag and return true
    if (active)
    {
        active = false;

        return true;
    }

    //otherwise return false
    return false;
}

void Keypress::checkIfActive()
{
    //if the keypress isn't yet active,
    //check whether there is a valid method
    //and if the current index has reached
    //the activation index
    //
    //if the keypress is active, no changes are made until
    //the flag is reset by isActive()
    if (!active)
    {
        active = (method.size() != 0 && index == activationIndex);
    }
}

void Keypress::updateIndex()
{
    //iterates the index forwards
    index++;

    //checks whether the keypress is now active
    checkIfActive();

    //reset the index if the end of the 
    //activation method has been reached
    if (index == method.size())
    {
        index = 0;
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
        //check to that the previous action is the same that at the
        //current index so as to make sure that the delta has correct meaning
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
            updateIndex();
        }
    }
}
