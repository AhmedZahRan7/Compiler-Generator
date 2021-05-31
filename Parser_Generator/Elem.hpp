#ifndef Elem_HPP
#define Elem_HPP
#include "utilities.hpp"

class Elem {
protected:
    string id;
public:
    Elem();
    Elem(string id);
    string getId() const;
    // bool operator==(Elem t) {return t.getId() == this->id;}
};

#endif
