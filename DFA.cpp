#include "DFA.hpp"

DFA::DFA(NFA* nfa, vector<string> inputSymbols) {
    build(nfa);
    this->inputSymbols = inputSymbols;
}

set<State*> stillUnmarked(map<set<State*>*, bool>& markedStates) {
    set<State*> emptySet; 
    for (auto s : markedStates) {
        if (s.second == false) {
            return *s.first;
        }
    }
    return emptySet;
}

void modifyAcceptingState(State* s, set<State*>& T) {
    for (State* t : T) {
        if (t->getIsAcceptingState() == true) {
            s->markAsAcceptingState();
            return;
        }
    }
    s->markAsNotAcceptingState();
}

void DFA::build(NFA* nfa) {
    this->startState = nfa->getStartState();
    set<State*> firstState = epsClosure(startState);
    states.insert(firstState);

    map<set<State*>*, bool> markedStates;
    markedStates[&firstState] = false;

    set<State*> T;
    while ((T = stillUnmarked(markedStates)).empty() == false) {
        markedStates[&T] = true;
        DStates[&T] = new State();
        modifyAcceptingState(DStates[&T], T);
        for (string a : inputSymbols) {
            set<State*> U = epsClosure(move(T, a));
            if (!U.empty() && states.find(U) == states.end()) {
                states.insert(U);
                markedStates[&U] = false;
                DStates[&U] = new State();
                modifyAcceptingState(DStates[&U], U);
            }
            transationsFromState[DStates[&T]].insert(new Transation(DStates[&T], DStates[&U], a));
        }
    }
}

set<State*>& DFA::epsClosure(State* s) {
    set<State*> nextStates;
    stack<State*> stack;
    stack.push(s);
    while (!stack.empty()) {
        State* t = stack.top();
        stack.pop();
        set<Transation*> transitions = transationsFromState[t];
        for (auto trans : transitions) {
            State* u = trans->to;
            if (trans->condition == EPSILON_TRANSATION && nextStates.find(u) == nextStates.end()) {
                nextStates.insert(u);
                stack.push(u);
            }
        }
    }
    return nextStates;
}

set<State*>& DFA::epsClosure(set<State*>& states) {
    set<State*> nextStates;
    for (auto s : states) {
        set<State*> tmp = epsClosure(s);
        if (!tmp.empty()) {
            nextStates.insert(tmp.begin(), tmp.end());
        }
    }
    return nextStates;
}

set<State*>& DFA::move(State* s, string a) {
    set<State*> nextStates;
    set<Transation*> transitions = transationsFromState[s];
    for (auto t : transitions) {
        if (t->condition == a) {
            nextStates.insert(t->to);
        }
    }
    return nextStates;
}

set<State*>& DFA::move(set<State*>& states, string a) {
    set<State*> nextStates;
    for (State* s : states) {
        set<State*> tmp = move(s, a);
        if (!tmp.empty()) {
            nextStates.insert(tmp.begin(), tmp.end());
        }
    }
    return nextStates;
}