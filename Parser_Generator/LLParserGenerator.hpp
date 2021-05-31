#ifndef LLParserGenerator_HPP
#define LLParserGenerator_HPP
#include "CFGBuilder.hpp"
#include "Production.hpp"

class LLParserGenerator {
private:
    vector<Production*> rules;
    vector<Terminal> symbols;
    // unordered_map<NonTerminal*, unordered_set<Terminal>> first, follow;
    map<NonTerminal*, set<Terminal*>> first, follow;
    void eliminateImmediateRecursion(Production* proc);
    void buildFirst();
    void buildFollow();

    void eliminateLeftRecursion();
    void leftRefactoring();
public:
    LLParserGenerator();
    LLParserGenerator(vector<Production*> procs);
};

#endif

