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
    vector<Production*> procList;

    // Rules mapping maps the left hand side non terminal string to a production.
    // It's used to add any replications of a rules to the same production.
    unordered_map<string, Production*> rulesMapping;

    unordered_map<string, Terminal*> terminalsMapping;
    unordered_map<string, NonTerminal*> nonTerminalsMapping;
    void buildCFG();
    void parseRule(string& line);
    string parseLHS(string& line, unsigned int& pos);
    vector<vector<Elem*>> parseRHS(string line);
    NonTerminal* addNonTerminal(string buffer);
    Terminal* checkTerminal(string buffer);
public:
    CFGBuilder(string path, set<TokenKey*> lexTokenKeys);
    void setTerminals(set<TokenKey*> lexTokenKeys);
    unordered_map<string, Terminal*> getTerminals();
    vector<Production*> getProcs();
    void printProductions();
};

#endif
