#include "Token.hpp"

set<TokenKey*>* Token::keys = new set<TokenKey*>;

Token::Token(TokenKey* key,TokenValue* value){
    this->key = key;
    this->value = value;
}
TokenKey* Token::getKey(){
    return this->key;
}
TokenValue* Token::getValue(){
    return this->value;
}
void Token::addTokenKey(TokenKey* key){
    keys->insert(key);
}
set<TokenKey*>* Token::getOrderedKeys(){
    return keys;
}