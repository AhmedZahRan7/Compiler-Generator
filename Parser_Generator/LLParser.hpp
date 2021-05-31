#ifndef LLParser_HPP
#define LLParser_HPP
#include "LLParserGenerator.hpp"

class LLParser {
private:
    map<NonTerminal*, map<Terminal*, Production*>> parsingTable;
public:
    LLParser();
};

#endif
