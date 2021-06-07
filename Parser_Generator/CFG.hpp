
#ifndef CFG_HPP
#define CFG_HPP
#include "Production.hpp"
#include "specialTerminals.hpp"

const string newNonTMark = "#";
const char factoringMark='?';
/**
 * @brief Context Free Grammar takes the rules and eliminate the left recursion, and
 * left refactoring from it.
*/
class CFG {
private:
    vector<Production*> rules;
    void eliminateLeftRecursion();
    void eliminateLeftRefactoring();
    void removeIndicesFromVector(std::vector<vector<Elem*>>& v, std::vector<int> rm );
    int countCommon(std::vector<int> commonElemContainer,vector<vector<Elem*>> rhs);
    vector<int> commonElemIndeces(vector<vector<Elem*>> rhs);
public:
    CFG();
    CFG(vector<Production*> rules);
    vector<Production*> getProcs();
};

#endif
