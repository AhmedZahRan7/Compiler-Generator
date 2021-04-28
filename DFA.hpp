#ifndef DFA_HPP
#define DFA_HPP
#include "NFA.hpp"

class DFA{
private:
    State *startState;
    set<State*> states;
    map<State*,set<Transation*>> transationsFromState;
public:
    DFA();
    DFA(NFA* nfa);
    void minimize();
};
#endif