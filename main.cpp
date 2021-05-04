#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include "NFA.hpp"
#include "Token.hpp"
#include "NFABuilder.hpp"
int main() {
    NFA* nfa1 = new NFA("a");
    NFA* nfa2 = new NFA("b");
    NFA* nfa3 = new NFA("c");
    cout<<NFA::convertIntoPositiveClosure(NFA::concatenate(NFA::unionWith(nfa1,nfa2),nfa3))->toString();
    return 0;
}
