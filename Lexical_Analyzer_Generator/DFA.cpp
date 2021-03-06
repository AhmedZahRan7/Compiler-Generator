#include "DFA.hpp"

DFA::DFA(NFA* nfa) {
    this->switchToDtran = false;
    build(nfa);
    this->switchToDtran = true;
    minimize();
}

set<State*> getUnmarked(map<set<State*>, bool>& markedStates) {
    set<State*> emptySet;
    for (auto s : markedStates) {
        if (s.second == false) {
            return s.first;
        }
    }
    return emptySet;
}

void modifyAcceptingState(State* s, set<State*>& T) {
    for (State* t : T) {
        if (t->getIsAcceptingState() == true) {
            s->markAsAcceptingState();
            s->setAcceptingTokenKey(t->getAcceptingTokenKey());
            return;
        }
    }
    s->markAsNotAcceptingState();
}

void DFA::build(NFA* nfa) {
    this->Ntransitions = nfa->getTransitionTable();
    this->inputSymbols = nfa->getInputSymbols();

    set<State*> T = epsClosure(nfa->getStartState());
    groupedStates[T] = new State();
    this->startState = groupedStates[T];

    set<set<State*>> statesBeforeMapping;
    statesBeforeMapping.insert(T);

    map<set<State*>, bool> markedStates;
    markedStates[T] = false;

    while (!T.empty()) {
        // cout << "T: " << printStates(T);
        markedStates[T] = true;
        modifyAcceptingState(groupedStates[T], T);
        for (string a : inputSymbols) {
            set<State*> U = epsClosure(move(T, a));
            if (!U.empty()) {
                // cout << "On input " << a << ": U = "; printStates(U);
                if (statesBeforeMapping.find(U) == statesBeforeMapping.end()) {
                    statesBeforeMapping.insert(U);
                    markedStates[U] = false;
                    groupedStates[U] = new State();
                    modifyAcceptingState(groupedStates[U], U);
                }
                Dtransitions[groupedStates[T]].insert(new Transation(groupedStates[T], groupedStates[U], a));
            }
        }
        T = getUnmarked(markedStates);
    }
    //cout << "Size before minmization: " << groupedStates.size() << endl;
}

set<State*> DFA::epsClosure(State* s) {
    set<State*> nextStates = {s};
    stack<State*> stack;
    stack.push(s);
    while (!stack.empty()) {
        State* t = stack.top();
        stack.pop();
        set<Transation*> transitions = switchToDtran ? Dtransitions[t] : Ntransitions[t];
        for (auto trans : transitions) {
            State* u = trans->to;
            if (trans->condition == EPSILON_TRANSATION && u != nullptr && nextStates.find(u) == nextStates.end()) {
                nextStates.insert(u);
                stack.push(u);
            }
        }
    }
    return nextStates;
}

set<State*> DFA::epsClosure(set<State*> states) {
    set<State*> nextStates(states.begin(), states.end());
    for (auto s : states) {
        set<State*> tmp = epsClosure(s);
        nextStates.insert(tmp.begin(), tmp.end());
    }
    return nextStates;
}

set<State*> DFA::move(State* s, string a) {
    set<State*> nextStates;
    set<Transation*> transitions = switchToDtran ? Dtransitions[s] : Ntransitions[s];
    for (auto t : transitions) {
        if (t->condition == a) {
            nextStates.insert(t->to);
        }
    }
    return nextStates;
}

set<State*> DFA::move(set<State*> states, string a) {
    set<State*> nextStates;
    for (State* s : states) {
        set<State*> tmp = move(s, a);
        if (!tmp.empty()) {
            nextStates.insert(tmp.begin(), tmp.end());
        }
    }
    return nextStates;
}

void DFA::minimize(){
    set<set<State*>> groups;
    unordered_map<TokenKey*, set<State*>> subgroups;
    TokenKey* nonAccepting = new TokenKey("NonAccepting");
    for (auto s : groupedStates) {
        State* curr = s.second;
        if (curr->getIsAcceptingState() == false) {
            subgroups[nonAccepting].insert(curr);
            // cout << "Not accepting set : " << curr->getID() << " -> "; printStates(s.first);
        } else {
            subgroups[curr->getAcceptingTokenKey()].insert(curr);
            // cout << curr->getAcceptingTokenKey()->getKey() << " set : " << curr->getID() << " -> "; printStates(s.first);
        }
    }
    for (auto entry : subgroups) {
        groups.insert(entry.second);
        // cout << "Group "; printStates(entry.second);
    }

    bool NotMinimized = true;
    while (NotMinimized){
        // cout << "Size now : " << groups.size() << '\n';
        set<set<State*>> newGroups(groups.begin(), groups.end());
        for (string x : inputSymbols) {
            newGroups = partition(newGroups, x);
        }
        if (newGroups == groups) {
            NotMinimized = false;
        }
        groups = newGroups;
    }
    mapGroupsToStates(groups);
    //cout << "Size after minimization: " << DStates.size() << endl;
}

set<set<State*>> DFA::partition(set<set<State*>>& groups, string x) {
    set<set<State*>> newGroups;
    for (set<State*> T : groups) {
        // cout << "Current Group: " << printStates(T);
        unordered_map<State*, bool> markedStates;
        for (State* s : T) {
            markedStates[s] = false;
        }

        set<State*>::iterator it;
        for (it = T.begin(); it != T.end(); ++it) {
            if (markedStates[*it]) continue;
            set<State*> subgroup;
            subgroup.insert(*it);
            markedStates[*it] = true;
            set<State*>::iterator jt = next(it, 1);
            for (; jt != T.end(); ++jt) {
                if (markedStates[*jt]) continue;
                if (goToSameGroup(groups, *it, *jt, x)) {
                    subgroup.insert(*jt);
                    markedStates[*jt] = true;
                }
            }
            // printStates(subgroup);
            newGroups.insert(subgroup);
        }
    }
    return newGroups;
}

bool DFA::goToSameGroup(set<set<State*>>& groups, State* a ,State* b, string s) {
    set<State*> nextA = move(a, s);
    set<State*> nextB = move(b, s);
    if (nextA.empty() && nextB.empty()) return true;
    else if ((!nextA.empty() && nextB.empty()) || (nextA.empty() && !nextB.empty())) return false;
    else {
        State* nexta = *nextA.begin();
        State* nextb = *nextB.begin();
        for (set<State*> g : groups)
            if (g.find(nexta) != g.end() && g.find(nextb) != g.end()) return true;
    }
    return false;
}

void DFA::mapGroupsToStates(set<set<State*>>& groups) {
    unordered_map<State*, State*> representativeStates;
    for (set<State*> T : groups) {
        State* repState = *T.begin();
        representativeStates[repState] = repState;
        for (auto it = next(T.begin(), 1); it != T.end(); ++it) {
            representativeStates[*it] = repState;
        }
    }

    for (auto s : representativeStates) DStates.insert(s.second);

    map<State*, set<Transation*>> finalTransitions;
    for (set<State*> T : groups) {
        State* repState = representativeStates[*T.begin()];
        for (auto tran : Dtransitions[repState]) {
            finalTransitions[repState].insert(new Transation(repState, representativeStates[tran->to], tran->condition));
        }
    }
    Dtransitions = finalTransitions;
    //this->toCSV();
}

map<State*, set<Transation*>> DFA::getTransitions() {return this->Dtransitions;}

set<State*> DFA::getStates() {return DStates;}

State* DFA::getStartState() {return this->startState;}

string DFA::printStates(set<State*> T) {
    stringstream out;
    out << '(';
    for (auto s : T) {
        out << s->getID() << " ";
    }
    out << ")\n";
    return out.str();
}

string DFA::toString() {
    stringstream out;
    out << "DFA :\nStart state is "+ this->startState->getID() + "\n";
    for(auto p : Dtransitions){
        out<<p.first->getID()<<"  \n";
        for(Transation* trans : p.second) {
            if(trans->to != NULL){
                out<<"--"<<trans->condition<<"-->"<<trans->to->getID();
                if(trans->to->getIsAcceptingState()) out<<" ("<<trans->to->getAcceptingTokenKey()->getKey()<<")";
                out<<"  \n";
            }
        }
    }
    return out.str();
}

void DFA::toCSV() {
    ofstream file("TransitionTable.csv");
    file << "State, ";
    for (string a : inputSymbols) file << a << ',';
    file << endl;
    stack<State*> st;
    st.push(this->startState);
    set<State*> noRepeatSet;
    while (!st.empty()) {
        State* curr = st.top();
        noRepeatSet.insert(curr);
        st.pop();
        file << curr->getID() << ',';
        for (string a : inputSymbols) {
            set<State*> nextSet = move(curr, a);
            if (!nextSet.empty()) {
                State* next = *nextSet.begin();
                if (noRepeatSet.find(next) == noRepeatSet.end()) {
                    st.push(next);
                    noRepeatSet.insert(next);
                }
                file << next->getID();
            }
            file << ',';
        }
        file << endl;
    }
    file.close();
}
