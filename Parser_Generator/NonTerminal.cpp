#include "NonTerminal.hpp"

NonTerminal::NonTerminal(string id) {
    this->id = id;
}

string NonTerminal::getID() {return this->id;}
