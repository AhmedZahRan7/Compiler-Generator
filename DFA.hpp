#ifndef DFA_HPP
#define DFA_HPP
#include "NFA.hpp"
#include "Token.hpp"

class DFA{
private:
    State *startState;
    set<set<State*>> states;
    map<State*,set<Transation*>> transationsFromState;
    map<set<State*>*, State*> DStates;
    vector<string> inputSymbols;
    vector<Token*> AcceptedTokens;
    vector<Token*> SymbolTable;
    fstream file;

    void build(NFA* nfa);
    set<set<State*>> partition(set<set<State*>>& groups, string x);
    bool NextToSameGroup(set<set<State*>>& groups, State* a ,State* b, string s);
    void minimize();
    set<State*> epsClosure(State* s);
    set<State*> epsClosure(set<State*>& states);
    set<State*> move(State* s, string a);
    set<State*> move(set<State*>& states, string a);
    void buildSymbolTable();
    void parseSrcProgram(string path);
    State* simulate(string x);
public:
    /**
     * @brief Construct a new DFA object from a given NFA machine
     * using the Subset construction algorithm
     * @param nfa 
     */
    DFA(NFA* nfa, string src_prog_path);
};
#endif