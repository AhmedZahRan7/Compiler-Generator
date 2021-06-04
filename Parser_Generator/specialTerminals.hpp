#ifndef SPECIAL_TERMINALS_HPP
#define SPECIAL_TERMINALS_HPP
#include "Terminal.hpp"
#include "NonTerminal.hpp"

extern Terminal* EPSILON;
extern Terminal* DOLLAR_SIGN;
extern Elem* SYNC;
bool isTerminal(Elem* e);
bool isNonTerminal(Elem* e);
bool isSync(Elem* e);
bool isEpsilon(Elem* e);
#endif
