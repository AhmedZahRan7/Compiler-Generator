#ifndef CFGBuilder_HPP
#define CFGBuilder_HPP
#include "utilities.hpp"
#include "Production.hpp"
#include "TokenKey.hpp"
#include "specialTerminals.hpp"

/**
 * @brief Context Free Grammar Builder takes the rules text file as an input,
 * checks the correctness of these rules with respect to the lexical analyzer generator's rules,
 * then outputs them to the parser generator.
*/
class CFGBuilder {
private:
    fstream rulesFile;
    
    unordered_map<string,Production*> rulesMapping;
    
    set<NonTerminal*> allNonTerminals;
    void buildCFG();
    void parseRule(string& line);
    string parseLHS(string& line, unsigned int& pos);
    vector<vector<Elem*>> parseRHS(string line);
    NonTerminal* addNonTerminal(string buffer);
    Terminal* checkTerminal(string buffer);
public:
    vector<Production*> procList;
    set<Terminal*> allTerminals;
    
    CFGBuilder(string path, set<TokenKey*> lexTokenKeys);
    void setTerminals(set<TokenKey*> lexTokenKeys);
    void printProductions();   
};

#endif
