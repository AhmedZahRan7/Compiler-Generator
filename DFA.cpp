#include "DFA.hpp"

DFA::DFA(NFA* nfa, string src_prog_path) {
    build(nfa);
    parseSrcProgram(src_prog_path);
}

set<State*> stillUnmarked(map<set<State*>, bool>& markedStates) {
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
            return;
        }
    }
    s->markAsNotAcceptingState();
}

void DFA::build(NFA* nfa) {
    this->startState = nfa->getStartState();
    set<State*> firstState = epsClosure(startState);
    states.insert(firstState);

    map<set<State*>, bool> markedStates;
    markedStates[firstState] = false;

    set<State*> T = stillUnmarked(markedStates);
    State* s = *T.begin();
    cout << s->getID() << endl;
    while (!T.empty()) {
        markedStates[T] = true;
        DStates[&T] = new State();
        modifyAcceptingState(DStates[&T], T);
        for (string a : inputSymbols) {
            set<State*> tmp = move(T, a);
            set<State*> U = epsClosure(tmp);
            if (!U.empty() && states.find(U) == states.end()) {
                states.insert(U);
                markedStates[U] = false;
                DStates[&U] = new State();
                modifyAcceptingState(DStates[&U], U);
            }
            transationsFromState[DStates[&T]].insert(new Transation(DStates[&T], DStates[&U], a));
        }
        T = stillUnmarked(markedStates);
    }
}

set<set<State*>> DFA::partition(set<set<State*>>& groups, string x) {
    set<set<State*>> newGroups;
    for (set<State*> T : groups) {
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
            set<State*>::iterator jt;
            for (jt = next(it, 1); jt != T.end(); ++jt) {
                if (markedStates[*jt]) continue;
                if (NextToSameGroup(groups, *it, *jt,x)) {
                    subgroup.insert(*jt);
                    markedStates[*jt] = true;
                }
            }
            newGroups.insert(subgroup);
        }
    }
    return newGroups;
}

bool DFA::NextToSameGroup(set<set<State*>>& groups, State* a ,State* b, string s) {
    set<State*>::iterator it = move(a, s).begin();
    State* nextOf_a= *it;
    it = move(b, s).begin();
    State* nextOf_b= *it;
    
    for (set<State*> g : groups) {
        if (g.find(nextOf_a) != g.end() && g.find(nextOf_b) != g.end()) {
            return true;
        }
    }
    return false;
}

void DFA::minimize(){
    set<set<State*>> groups;
    unordered_map<TokenKey*, set<State*>> subgroups;
    TokenKey* nonAccepting = new TokenKey("NonAccepting");
    for (auto s : DStates) {
        State* curr = s.second;
        if (curr->getIsAcceptingState() == false) {
            subgroups[nonAccepting].insert(curr);
        } else {
            subgroups[curr->getAcceptingTokenKey()].insert(curr);
        }
    }
    for (auto entry : subgroups) {
        groups.insert(entry.second);
    }
    
    bool NotMinimized = true;
    while (NotMinimized){
        set<set<State*>> newGroups(groups.begin(), groups.end());
        for (string x : inputSymbols) {
            newGroups = partition(newGroups, x);
        }
        if (newGroups == groups) {
            NotMinimized = false;
        }
    }
}

set<State*> DFA::epsClosure(State* s) {
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

set<State*> DFA::epsClosure(set<State*>& states) {
    set<State*> nextStates;
    for (auto s : states) {
        set<State*> tmp = epsClosure(s);
        if (!tmp.empty()) {
            nextStates.insert(tmp.begin(), tmp.end());
        }
    }
    return nextStates;
}

set<State*> DFA::move(State* s, string a) {
    set<State*> nextStates;
    set<Transation*> transitions = transationsFromState[s];
    for (auto t : transitions) {
        if (t->condition == a) {
            nextStates.insert(t->to);
        }
    }
    return nextStates;
}

set<State*> DFA::move(set<State*>& states, string a) {
    set<State*> nextStates;
    for (State* s : states) {
        set<State*> tmp = move(s, a);
        if (!tmp.empty()) {
            nextStates.insert(tmp.begin(), tmp.end());
        }
    }
    return nextStates;
}

vector<string> tokenize(string s, string del = " ") {
    string buffer;
    vector<string> tokens;
    for (char c : s) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')|| (c >= '0' && c <= '9')) {
            buffer += c;
        } else {
            tokens.emplace_back(buffer);
            buffer = "";
            tokens.emplace_back(string(1, c));
        }
    }
    return tokens;
}

void DFA::buildSymbolTable() {
    for (Token* t : AcceptedTokens) {
        if (t->getKey() == new TokenKey("id") || t->getKey() == new TokenKey("identifier")) {
            SymbolTable.emplace_back(t);
        }
    }
}

void DFA::parseSrcProgram(string path) {
    this->file.open(path,ios::in);
    if (!this->file.is_open()) cout << "File not exist";
    string line;
    while (getline(this->file,line)) {
        vector<string> tokens = tokenize(line);
        for (string x : tokens) {
            State* s = simulate(x);
            if (s->getIsAcceptingState() == true) {
                AcceptedTokens.push_back(new Token(s->getAcceptingTokenKey(), new TokenValue(x)));
            }
        }
    }

    // Printing Accepted Tokens for debugging
    for (auto t : AcceptedTokens) {
        cout << "Token key: " << t->getKey()->getKey() << "Token Value: " << t->getValue() << endl;
    }

    buildSymbolTable();
}

State* DFA::simulate(string x) {
    State* s = startState;
    int i = 0;
    string c = string(1, x[i++]);
    while (c[0] != EOF) {
        auto it = move(s, c).begin();
        s = *it;
        c = string(1, x[i++]);
    }
    return s;
}
