#ifndef LEXICAL_ANALYZER_GENERATOR
#define LEXICAL_ANALYZER_GENERATOR
#include "NFABuilder.hpp"
#include "DFA.hpp"
#include "Lexical_Analyzer.hpp"

class Lexical_Analyzer_Generator {
private:
    Lexical_Analyzer* lex;
public:
    Lexical_Analyzer_Generator(string rulesFilePath);
    Lexical_Analyzer* get_Lexical_Analyzer();
};
#endif
