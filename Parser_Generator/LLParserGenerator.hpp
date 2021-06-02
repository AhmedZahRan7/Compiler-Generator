#ifndef LLParserGenerator_HPP
#define LLParserGenerator_HPP
#include "CFGBuilder.hpp"
#include "Production.hpp"
#include "LLParser.hpp"
#include "SpecialTerminals.hpp"

class LLParserGenerator {
private:
    vector<Production*> rules;
    vector<Terminal> symbols;
    set<Terminal*> allTerminals;
    unordered_map<NonTerminal*, unordered_set<Terminal*>> first, follow;
    unordered_map<NonTerminal*, bool> isFirstBuild;
    unordered_map<NonTerminal*, Production*> rulesMapping;
    void buildFirst();
    void buildFollow();
    void eliminateImmediateRecursion();
    void eliminateLeftRecursion();
    void leftRefactoring();
    void setFirst(NonTerminal* lhs);
    unordered_set<Terminal*> getFirstOfNonTerminal(NonTerminal* nt, bool& epsilonExist);
    void followNonTerInLast(NonTerminal* lhs , NonTerminal* rhs);
    void followTer(NonTerminal* nonTerminal,Terminal* terminal);
    void followNonTer(NonTerminal* lhs ,NonTerminal* rhs, NonTerminal* nextOfRhs);
    unordered_set<Terminal*> getFirstOfOneProduction(vector<Elem*> elements);
public:
    LLParserGenerator();
    LLParserGenerator(vector<Production*> rules, set<Terminal*> allTerminals);
    LLParser* generateParser();
    void printFirst();
    void printFollow();
};

#endif

