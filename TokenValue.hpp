#ifndef TOKEN_VALUE_HPP
#define TOKEN_VALUE_HPP
#include "utilities.hpp"
class TokenValue{
private:
    string val;
public:
    TokenValue(string val);
    string getValue();
};


#endif