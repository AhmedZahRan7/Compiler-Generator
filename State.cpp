//implementation of state class
#include "State.hpp"

State::State(){
    this->id = generateID();
}

bool State::getIsAcceptingState(){
    return this->isAcceptingState;
}

void State::markAsAcceptingState(){
    this->isAcceptingState = true;
}

void State::markAsNotAcceptingState(){
    this->isAcceptingState = false;
}

string State::getID(){
    return this->id;
}