#ifndef TRANS_HPP
#define TRANS_HPP
#include "State.hpp"

class Transation{
public:
    State *from,*to;
    string condition;
    Transation();
    Transation(State* from,State* to,string condition);
};

#endif