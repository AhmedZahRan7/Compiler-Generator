#ifndef LLParser_HPP
#define LLParser_HPP
#include "LLParserGenerator.hpp"

class LLParser {
private:
    map<NonTerminal*, map<Terminal, NonTerminal*>> parsingTable;
public:
    LLParser();
};

#endif
