#ifndef STATE_HPP
#define STATE_HPP
#include "utilities.hpp"
#include "TokenKey.hpp"
class State{
private:
    string id;
    bool isAcceptingState;
    TokenKey* acceptingTokenKey; 
public:
    State();
    State(string a);
    bool getIsAcceptingState();
    void markAsAcceptingState();
    void markAsNotAcceptingState();
    TokenKey* getAcceptingTokenKey();
    void setAcceptingTokenKey(TokenKey* tokenKey);
    string getID();
};
#endif