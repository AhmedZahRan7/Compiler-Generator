#include <bits/stdc++.h>
#include "NFA.hpp"
#include "NFABuilder.hpp"
#include "DFA.hpp"

using namespace std;

NFA* createLectureExample() {
    State* zero = new State("0");
    State* one = new State("1");
    State* two = new State("2");
    State* three = new State("3");
    State* four = new State("4");
    State* five = new State("5");
    State* six = new State("6");
    State* seven = new State("7");
    State* eight = new State("8");
    State* nine = new State("9");
    State* ten = new State("10");

    auto id = new TokenKey("id");
    three->setAcceptingTokenKey(id);
    three->markAsAcceptingState();
    seven->setAcceptingTokenKey(id);
    seven->markAsAcceptingState();
    ten->setAcceptingTokenKey(id);
    ten->markAsAcceptingState();

    auto nfa = new NFA();
    nfa->addState(zero);
    nfa->addState(one);
    nfa->addState(two);
    nfa->addState(three);
    nfa->addState(four);
    nfa->addState(five);
    nfa->addState(six);
    nfa->addState(seven);
    nfa->addState(eight);
    nfa->addState(nine);
    nfa->addState(ten);

    nfa->setStartState(zero);
    nfa->setFinalState(three);

    nfa->addTransation(zero, one, EPSILON_TRANSATION);
    nfa->addTransation(zero, four, EPSILON_TRANSATION);
    nfa->addTransation(zero, eight, EPSILON_TRANSATION);
    nfa->addTransation(one, two, "b");
    nfa->addTransation(two, three, "a");
    nfa->addTransation(four, five, "b");
    nfa->addTransation(five, six, "a");
    nfa->addTransation(six, seven, "b");
    nfa->addTransation(eight, nine, "b");
    nfa->addTransation(nine, nine, "b");
    nfa->addTransation(nine, ten, EPSILON_TRANSATION);
    nfa->addTransation(ten, ten, "a");
    return nfa;
}

int main() {
    NFABuilder* builder = new NFABuilder("lexical_rules.txt");
    auto nfa = builder->getNFA();
    
    // cout << nfa->toString();
    // auto nfa = createLectureExample();
    auto dfa = new DFA(nfa, "test_program.txt");
    dfa->simulate();

    return 0;
}
