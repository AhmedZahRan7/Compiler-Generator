#include "Token.hpp"

vector<TokenKey*>* Token::keys = new vector<TokenKey*>;

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
    keys->push_back(key);
}
vector<TokenKey*>* Token::getOrderedKeys(){
    return keys;
}