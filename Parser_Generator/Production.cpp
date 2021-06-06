#include "Production.hpp"

Production::Production() {}

Production::Production(NonTerminal* lhs, vector<vector<Elem*>>& rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

NonTerminal* Production::getLHS() {return this->lhs;}

vector<vector<Elem*>> Production::getRHS() {return this->rhs;}

void Production::setRHS(vector<vector<Elem*>> rhs) { this->rhs = rhs; }

void Production::addToRHS(vector<vector<Elem*>> anotherRHS){
    this->rhs.insert(this->rhs.end(), anotherRHS.begin(), anotherRHS.end());
}

string Production::toString() {
    stringstream out;
    out << this->lhs->getId() << " ---> ";
    for (unsigned int i = 0; i < this->rhs.size(); ++i) {
        for (Elem* e : this->rhs[i]) {
            out << e->getId() << " ";
        }
        if (i != rhs.size() - 1) out << "|| ";
    }
    out << endl;
    return out.str();
}
