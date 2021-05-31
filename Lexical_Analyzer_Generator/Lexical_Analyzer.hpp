#ifndef LEXICAL_ANALYZER
#define LEXICAL_ANALYZER
#include "NFA.hpp"
#include "Token.hpp"

class Lexical_Analyzer {
private:
    State* startState;
    set<State*> DStates;
    map<State*, set<Transation*>> Dtransitions;
    vector<Token*> Tokens;
    vector<Token*> SymbolTable;
    unsigned int token_index;

    State* simulate(string word);
    void simulate(vector<string> words);
    set<State*> move(State* s, string a);
    void addToken(State* s, string word);
    void recoveryRoutine(string word);

public:
    Lexical_Analyzer(set<State*> DStates, map<State*, set<Transation*>> Dtransitions, State* startState);
    void simulateProgramFile(string path);
    Token* getToken();
    vector<Token*> getSymbolTable();
    string printTransitionTable();
    set<TokenKey*>* getOrderedKeys();
};
#endif
