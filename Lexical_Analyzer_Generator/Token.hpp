#ifndef TOKEN_HPP
#define TOKEN_HPP
#include "TokenKey.hpp"
#include "TokenValue.hpp"

/**
 * @brief Token is the output unit of the lexical analyzer to be an input
 *  for the next step of the compiler (The Parser)
 * Token cosists of 2 components : 
 * @param key the key of the token 
 * @param value the value of the token
 * if the input of analyzer is "while (x>50) x = x - 1;"
 * the output tokens are <"keyword","while">, <"Punctuation","(">,
 *                       <"id","x">, <"relop",">">, <"num","50">,
 *                       <"Punctuation",")">, <"id","x">, <"assign","=">,
 *                       <"id","x">, <"addop","-">, <"num","1">, <"Punctuation",";">
 */
class Token {
private:
    TokenKey* key;
    TokenValue* value;
    static set<TokenKey*>* keys;
public:
    Token();
    Token(TokenKey* key,TokenValue* value);
    TokenKey* getKey();
    TokenValue* getValue();
    static void addTokenKey(TokenKey*);
    static set<TokenKey*>* getOrderedKeys();
};
#endif