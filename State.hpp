#ifndef STATE_HPP
#define STATE_HPP
#include "utilities.hpp"
#include "Token.hpp"
class State{
private:
    string id;
    bool isAcceptingState;
    Token* acceptingToken; 
public:
    State();
    bool getIsAcceptingState();
    void markAsAcceptingState();
    void markAsNotAcceptingState();
    Token* getAcceptingToken();
    void setAcceptingToken(Token* token);
    string getID();
};
#endif