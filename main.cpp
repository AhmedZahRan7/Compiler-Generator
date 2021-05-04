#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include "NFA.hpp"
#include "Token.hpp"
#include "NFABuilder.hpp"
int main() {
    NFABuilder * builder = new NFABuilder("lexical_rules.txt");
    // cout<<infixToPostfix("a-z | A-Z")<<endl;
    // cout<<infixToPostfix("letter (letter|digit)*")<<endl;
    // cout<<infixToPostfix("digit+")<<endl;
    // cout<<infixToPostfix("digit+ | digit+ . digits ( \\L | E digits) ")<<endl;
    // cout<<infixToPostfix("\\=\\= | !\\= | > | >\\= | < | <\\= ")<<endl;
    // cout<<infixToPostfix("\\* | /")<<endl;
    
    //test example is (a|b)*abb
    // NFA* nfa1 = ((new NFA("a"))->unionWith(new NFA("b")))->convertIntoKleeneclosure()->concatinateWith((new NFA("a"))->concatinateWith(new NFA("b"))->concatinateWith(new NFA("b")));
    cout<<builder->getNFA()->toString();
    // passed ✔
    return 0;
}
// | + 