#ifndef LLParser_HPP
#define LLParser_HPP
#include "NonTerminal.hpp"
#include "Terminal.hpp"

class LLParser {
private:
    unordered_map<NonTerminal*, unordered_map<Terminal*, vector<Elem*> >> parsingTable;
public:
    LLParser();
};

#endif
