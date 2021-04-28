#ifndef STATE_HPP
#define STATE_HPP
#include "utilities.hpp"

class State{
private:
    string id;
    bool isAcceptingState;
public:
    State();
    bool getIsAcceptingState();
    void markAsAcceptingState();
    void markAsNotAcceptingState();
    string getID();
};
#endif