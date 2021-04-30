#include "DFA.hpp"

DFA::DFA(NFA* nfa) {
    build(nfa);
}

set<State*>* stillUnmarked(map<set<State*>*, bool>& markedStates) {
    for (auto s : markedStates) {
        if (s.second == false) {
            return s.first;
        }
    }
    return nullptr;
}

void DFA::build(NFA* nfa) {
    this->startState = nfa->getStartState();
    set<State*>* firstState = epsClosure(startState);
    states.insert(firstState);

    map<set<State*>*, bool> markedStates;
    markedStates[firstState] = false;

    set<State*>* T;
    while ((T = stillUnmarked(markedStates)) != nullptr) {
        markedStates[T] = true;
        DStates[T] = new State();
        for (string a : *nfa->getInputSymbols()) {
            set<State*>* U = move(*T, a);
            if (states.find(U) == states.end()) {
                states.insert(U);
                markedStates[U] = false;
                DStates[U] = new State();
            }
            transationsFromState[DStates[T]].insert(new Transation(DStates[T], DStates[U], a));
        }
    }
}

set<State*>* DFA::epsClosure(State* s) {
    return move(s, EPSILON_TRANSATION);
}

set<State*>* DFA::epsClosure(set<State*>& states) {
    set<State*> nextStates;
    for (State* s : states) {
        set<State*>* firstState = epsClosure(s);
        if (!firstState->empty()) {
            nextStates.insert(firstState->begin(), firstState->end());
        }
    }
    return &nextStates;
}

set<State*>* DFA::move(State* s, string a) {
    set<State*> nextStates;
    set<Transation*> transitions = transationsFromState[s];
    for (auto t : transitions) {
        if (t->condition == a) {
            nextStates.insert(t->to);
        }
    }
    return &nextStates;
}

set<State*>* DFA::move(set<State*>& states, string a) {
    set<State*> nextStates;
    for (State* s : states) {
        set<State*>* firstState = move(s, a);
        if (!firstState->empty()) {
            nextStates.insert(firstState->begin(), firstState->end());
        }
    }
    return &nextStates;
}