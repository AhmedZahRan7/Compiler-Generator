#ifndef Terminal_HPP
#define Terminal_HPP
#include "Elem.hpp"
#include "utilities.hpp"

class Terminal : Elem {
private:
    string key;
public:
    Terminal();
    Terminal(string key);
    string getKey();

    // bool operator==(Terminal t) {return t.getKey() == this->key;}
};

#endif
