#ifndef NFABuilder_HPP
#define NFABuilder_HPP
#include "NFA.hpp"

class NFABuilder{
private:
    NFA *nfa;
    fstream file;
    void buildTheNfa();
    char getNextChar();
public:
    // NFABuilder();
    NFABuilder(string path);    
    NFA* getNFA();
};
#endif