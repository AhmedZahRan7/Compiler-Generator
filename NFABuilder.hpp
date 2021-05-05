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
    NFA* mergeNfaList();
    NFA* buildNFAFromPostfixExpression(vector<string> postfix);
    NFA* createNFAFromWord(string word);
    unordered_map<string,NFA*> regularDefinitionNFAs;
public:
    // NFABuilder();
    NFABuilder(string path);    
    NFA* getNFA();
};
#endif