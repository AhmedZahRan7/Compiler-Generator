#include "TokenKey.hpp"


TokenKey::TokenKey(string key){
    this->key = key;
}

string TokenKey::getKey(){
    return this->key;
}