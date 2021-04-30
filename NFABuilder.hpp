#ifndef NFABuilder_HPP
#define NFABuilder_HPP
#include "NFA.hpp"

class NFABuilder{
private:
    NFA *nfa;
    fstream file;
    void buildTheNfa();
    string getNextLine();
    void parseRule(string line);
public:
    // NFABuilder();
    NFABuilder(string path);    
    NFA* getNFA();
};
#endif