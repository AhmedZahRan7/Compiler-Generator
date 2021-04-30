#ifndef TOKEN_KEY_HPP
#define TOKEN_KEY_HPP
#include "utilities.hpp"

class TokenKey{
private:
    string key;
public:
    TokenKey();
    TokenKey(string key);
    string getKey();
};

#endif