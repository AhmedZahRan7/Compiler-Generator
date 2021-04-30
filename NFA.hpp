#ifndef NFA_HPP
#define NFA_HPP
#include "State.hpp"
#include "Transation.hpp"
const string EPSILON_TRANSATION = "EPS";

/**
    NFA class to represent the Non-Deterministic Finite State Machine, The basic step to
    build the NFA is to define the first transation then constract the whole FNA by constraction
    using 4 operations (Concatenation, Union, Kleene closure, Positive closure)

    @param statrtState the state where the NFA should start from.
    @param finalState the state where the NFA should end ie:There is no more AC transations.
    @param States set of states in this machine.
    @param transationsFromState maping between states and transations out of it.
*/
class NFA {
private:
    State *startState, *finalState;
    set<State*> states;
    map<State*,set<Transation*>> transationsFromState;
public:

    NFA();
    
    /**
        NFA consractor
        @param input the basic transation.
        for example if input is "a" then we build the following NFA,
        (Start) --a--> ((finalState))
        *double parenthesis indicates an Accepting state 
    */
    NFA(string input);

    /**
        concatinate the current NFA with another NFA.
        ex: if the current is  (Start) --a--> ((finalState))
        and the secondNFA is (Start) --b--> ((finalState))
        then the current conervted into (Start) --a-->(midState) --b--> ((finalState))
        @param secondNFA the another NFA to be concatinated with the current NFA.
    */
    void concatinateWith(NFA* secondNFA);

    /**
        union the current NFA with another NFA.
        @param secondNFA the another NFA to be unioned with the current NFA.
    */
    void unionWith(NFA* secondNFA);

    /**
        Kleene Closure of the current NFA
    */    
    void convertIntoKleeneclosure();
     /**
        Postive Closure of the current NFA
    */
    void convertIntoPostiveclosure();

    /**
       get NFA parameters as a string for debuging purposes 
    */
    string toString();

    // void generateTransitionTable();

    State* getStartState();
    vector<string>* getInputSymbols();
};
#endif