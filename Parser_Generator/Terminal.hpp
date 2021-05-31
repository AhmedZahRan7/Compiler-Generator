#ifndef Terminal_HPP
#define Terminal_HPP
#include "Elem.hpp"

class Terminal : Elem {
private:

public:
    Terminal();
    Terminal(string key);

    // bool operator==(Terminal t) {return t.getKey() == this->key;}
};

#endif
