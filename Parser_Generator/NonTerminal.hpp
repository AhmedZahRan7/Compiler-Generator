#ifndef NonTerminal_HPP
#define NonTerminal_HPP
#include "Elem.hpp"
#include "utilities.hpp"

class NonTerminal : Elem {
private:
    string id;
public:
    NonTerminal();
    NonTerminal(string id);
    string getID();
};

#endif
