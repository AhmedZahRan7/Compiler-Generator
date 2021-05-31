#ifndef LLParserGenerator_HPP
#define LLParserGenerator_HPP
#include "CFGBuilder.hpp"
#include "Production.hpp"
#include "LLParser.hpp"

class LLParserGenerator {
private:
    vector<Production*> rules;
    vector<Terminal> symbols;
    set<Terminal*> allTerminals;
    map<NonTerminal*, set<Terminal*>> first, follow;
    void buildFirst();
    void buildFollow();
    void eliminateImmediateRecursion();
    void eliminateLeftRecursion();
    void leftRefactoring();
public:
    LLParserGenerator();
    LLParserGenerator(vector<Production*> rules, set<Terminal*> allTerminals);
    LLParser* generateParser();
};

#endif

