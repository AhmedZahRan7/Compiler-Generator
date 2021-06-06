
#ifndef CFG_HPP
#define CFG_HPP
#include "Production.hpp"
#include "specialTerminals.hpp"

const string newNonTMark = "#";

/**
 * @brief Context Free Grammar takes the rules and eliminate the left recursion, and
 * left refactoring from it.
*/
class CFG {
private:
    vector<Production*> rules;
    void eliminateLeftRecursion();
    void eliminateLeftRefactoring();
public:
    CFG();
    CFG(vector<Production*> rules);
    vector<Production*> getProcs();
};

#endif
