#include "specialTerminals.hpp"

Terminal* EPSILON = new Terminal("Epsilon");
Terminal* DOLLAR_SIGN  = new Terminal("$");
Elem* SYNC = new Elem("sync");

bool isTerminal(Elem* e) { return dynamic_cast<Terminal*>(e) != nullptr; }
bool isNonTerminal(Elem* e) { return dynamic_cast<NonTerminal*>(e) != nullptr; }
bool isSync(Elem* e) { return e == SYNC; }
bool isEpsilon(Elem* e){ return e == EPSILON; }
bool isDollarSign(Elem* e) {return e == DOLLAR_SIGN; }
