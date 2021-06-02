#ifndef LEXICAL_ANALYZER
#define LEXICAL_ANALYZER
#include "NFA.hpp"
#include "Token.hpp"

class Lexical_Analyzer {
private:
    State* startState;
    set<State*> DStates;
    map<State*, set<Transation*>> Dtransitions;
    queue<Token*> tokenBuffer;
    vector<Token*> SymbolTable;

    State* simNormal(string word);
    set<State*> move(State* s, string a);
    void addToken(State* s, string word);
    void simRecovery(string word);
public:
    Lexical_Analyzer(set<State*> DStates, map<State*, set<Transation*>> Dtransitions, State* startState);
    Token* getNextToken();
    void simulate(string word);
    vector<Token*> getSymbolTable();
    string printTransitionTable();
    set<TokenKey*>* getOrderedKeys();
};
#endif
