#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include "NFA.hpp"
int main() {
    NFA* nfa1 = new NFA("a");
    NFA* nfa2 = new NFA("b");
    nfa1->concatinateWith(nfa2);
    cout<<nfa1->toString();
    return 0;
}