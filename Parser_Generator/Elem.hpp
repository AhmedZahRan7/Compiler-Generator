#ifndef Elem_HPP
#define Elem_HPP
#include "utilities.hpp"

class Elem {
protected:
    string id;
public:
    Elem();
    Elem(string id);
    virtual string getId() const;
};

extern Elem* SYNC;

#endif
