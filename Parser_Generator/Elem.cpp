#include "Elem.hpp"

Elem::Elem(string id) {this->id = id;}

string Elem::getId() const {return this->id;}

Elem* SYNC = new Elem("sync");
