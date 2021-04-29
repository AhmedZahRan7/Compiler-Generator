#ifndef DFA_HPP
#define DFA_HPP
#include "NFA.hpp"

class DFA{
private:
    State *startState;
    set<State*> states;
    map<State*,set<Transation*>> transationsFromState;
public:
    // DFA();
    /**
     * @brief Construct a new DFA object from a given NFA machine
     * using the Subset construction algorithm
     * @param nfa 
     */
    DFA(NFA* nfa);
    void minimize();
};
#endif