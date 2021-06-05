#ifndef LLParser_HPP
#define LLParser_HPP
#include "NonTerminal.hpp"
#include "Terminal.hpp"
#include "Token.hpp"
#include "specialTerminals.hpp"

class LLParser {
private:
    unordered_map<NonTerminal*, unordered_map<Terminal*, vector<Elem*> >> parsingTable;
    stack<Elem*> LLStack;
    unordered_map<string, Terminal*> terminalsMapping;
public:
    LLParser();
    LLParser(NonTerminal* startState, unordered_map<string, Terminal*> terminalsMapping);
    void addToTable(NonTerminal* lhs, Terminal* symbol, vector<Elem*> elements);
    bool existInTable(NonTerminal* lhs, Terminal* symbol);
    void parse(Token* input);
    void output(NonTerminal* lhs, Terminal* input);

};

#endif
