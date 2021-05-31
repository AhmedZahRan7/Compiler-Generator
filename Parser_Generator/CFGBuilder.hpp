#ifndef CFGBuilder_HPP
#define CFGBuilder_HPP
#include "utilities.hpp"

/**
 * @brief Context Free Grammar Builder takes the rules text file as an input,
 * checks the correctness of these rules with respect to the lexical analyzer generator's rules,
 * then outputs them to the parser generator.
*/
class CFGBuilder {
private:
    fstream rulesFile;
    void buildCFG();
    bool isAllLexTokensIncluded();
public:
    CFGBuilder(string path);
};

#endif
