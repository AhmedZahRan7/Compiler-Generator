#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include "NFA.hpp"
#include "Token.hpp"
#include "NFABuilder.hpp"
int main() {
    // NFABuilder * builder = new NFABuilder("lexical_rules.txt");
    //test example is (a|b)*abb
    NFA* nfa1 = ((new NFA("a"))->unionWith(new NFA("b")))->convertIntoKleeneclosure()->concatinateWith((new NFA("a"))->concatinateWith(new NFA("b"))->concatinateWith(new NFA("b")));
    cout<<nfa1->toString();
    // passed ✔
    return 0;
}