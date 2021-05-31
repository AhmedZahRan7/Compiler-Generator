#include "Production.hpp"

Production::Production() {}

Production::Production(NonTerminal* lhs, vector<vector<Elem*>> rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

NonTerminal* Production::getLHS() {return this->lhs;}

vector<vector<Elem*>> Production::getRHS() {return this->rhs;}
