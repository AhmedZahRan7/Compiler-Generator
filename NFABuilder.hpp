#ifndef NFABuilder_HPP
#define NFABuilder_HPP
#include "NFA.hpp"
#include "Token.hpp"
enum RuleType{
    KeyWord,
    Punctuation,
    RegularDefinition,
    RegularExpression,
};

class NFABuilder{
private:
    NFA* nfa;
    vector<NFA*> nfaList;
    fstream file;
    void buildTheNfa();
    string getNextLine();
    void parseRule(string& line);
    RuleType detectRuleType(string& line);
    void keyWordParser(string& line);
    void punctuationParser(string& line);
    void regularDefinitionParser(string& line);
    void regularExpressionParser(string& line);
    NFA* formNfaForKeyWord(string word,TokenKey* key);
    void mergeNfaList();
    NFA* buildNFAFromPostfixExpression(vector<string> postfix);
    unordered_map<string,NFA*> regularDefinitionNFAs;
public:
    // NFABuilder();
    NFABuilder(string path);    
    NFA* getNFA();
};
#endif