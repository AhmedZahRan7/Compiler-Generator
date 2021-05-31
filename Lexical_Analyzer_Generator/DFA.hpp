#ifndef DFA_HPP
#define DFA_HPP
#include "NFA.hpp"
#include "Token.hpp"

/**
 * @brief Deterministic finite state machine constructed from an NFA,
 * then minimized and used in lexical analyzer.
 */
class DFA{
private:
    State* startState;
    map<State*, set<Transation*>> Ntransitions;
    map<State*, set<Transation*>> Dtransitions;
    /**
     * @brief set of nfa states belong to one group are mapped to a new state which
     * will be used instead of the set.
     */
    map<set<State*>, State*> groupedStates;
    set<State*> DStates;
    set<string> inputSymbols;
    bool switchToDtran;

    /**
     * @brief Build the dfa using subset construction algorithm.
     */
    void build(NFA* nfa);
    set<State*> epsClosure(State* s);
    set<State*> epsClosure(set<State*> states);
    set<State*> move(State* s, string a);
    set<State*> move(set<State*> states, string a);
    void minimize();
    set<set<State*>> partition(set<set<State*>>& groups, string x);
    bool goToSameGroup(set<set<State*>>& groups, State* a ,State* b, string s);
    void mapGroupsToStates(set<set<State*>>& groups);
    string printStates(set<State*> T);
public:
    /**
     * @brief Construct a new DFA object from a given NFA machine
     * using the Subset construction algorithm.
     *
     * @param src_prog_path path to a txt file of a source program that
     * wil be parsed and converted to tokens.
     */
    DFA(NFA* nfa);
    map<State*, set<Transation*>> getTransitions();
    set<State*> getStates();
    State* getStartState();
    string toString();
    void toCSV();
};
#endif
