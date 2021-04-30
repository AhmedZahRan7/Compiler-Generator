#ifndef TOKEN_HPP
#define TOKEN_HPP
#include "TokenKey.hpp"
#include "TokenValue.hpp"

class Token {
private:
    TokenKey key;
    TokenValue value;
    static vector<TokenKey> keys;
public:
    Token();
    Token(TokenKey key,TokenValue value);
    TokenKey getKey();
    TokenValue getValue();
    static vector<TokenKey> getOrderedKeys();
};
#endif