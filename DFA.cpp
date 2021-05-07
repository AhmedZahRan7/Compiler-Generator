#include "DFA.hpp"

DFA::DFA(NFA* nfa, string src_prog_path) {
    parseSrcProgram(src_prog_path);
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
    DStates[T] = new State();
    this->startState = DStates[T];

    set<set<State*>> statesBeforeMapping;
    statesBeforeMapping.insert(T);

    map<set<State*>, bool> markedStates;
    markedStates[T] = false;

    while (!T.empty()) {
        // cout << "T: " << printStates(T);
        markedStates[T] = true;
        modifyAcceptingState(DStates[T], T);
        for (string a : inputSymbols) {
            set<State*> U = epsClosure(move(T, a));
            if (!U.empty()) {
                // cout << "On input " << a << ": U = "; printStates(U);
                if (statesBeforeMapping.find(U) == statesBeforeMapping.end()) {
                    statesBeforeMapping.insert(U);
                    markedStates[U] = false;
                    DStates[U] = new State();
                    modifyAcceptingState(DStates[U], U);
                }
                Dtransitions[DStates[T]].insert(new Transation(DStates[T], DStates[U], a));
            }
        }
        T = getUnmarked(markedStates);
    }
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
    for (auto s : DStates) {
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
        set<set<State*>> newGroups(groups.begin(), groups.end());
        for (string x : inputSymbols) {
            newGroups = partition(newGroups, x);
        }
        if (newGroups == groups) {
            NotMinimized = false;
        }
        groups = newGroups;
    }
}

set<set<State*>> DFA::partition(set<set<State*>>& groups, string x) {
    set<set<State*>> newGroups;
    for (set<State*> T : groups) {
        // cout << "Current Group: "; printStates(T);
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

void DFA::recoveryRoutine(string word) {
    State* s = this->startState;
    set<State*> curr_set;
    string buffer;
    pair<State*, int> lastAccept;
    bool wordFinished = false;
    while (!wordFinished) {
        for (int i = 0; i < word.size() && !word.empty(); ++i) {
            buffer += word[i];
            curr_set = move(s, string(1, word[i]));
            if (curr_set.empty()) {
                if (lastAccept.second == 0) {    // No accepting state is found til now
                    cout << word << " is not identified so neglected.\n";
                    return; // If input prefix doesn't move to any state then the rest of the word neglected
                } else {
                    buffer = "";
                    addToken(s, word.substr(0, lastAccept.second + 1));
                    word.erase(0, lastAccept.second + 1);
                    lastAccept = {};
                    i = -1; // Will be incremented in the for loop
                }
                s = this->startState;
            } else {
                s = *curr_set.begin();
                if (s->getIsAcceptingState()) {
                    lastAccept = {s, i};
                }
            }
        }
        if (lastAccept.second == 0) {
            if (!buffer.empty()) cout << buffer << " is not identified so neglected.\n";
            wordFinished = true;
        } else {
            buffer = "";
            addToken(s, word.substr(0, lastAccept.second + 1));
            word.erase(0, lastAccept.second + 1);
            lastAccept = {};
        }
    }
}

State* DFA::simulate(string word) {
    State* s = this->startState;
    set<State*> curr_set;
    for (char c : word) {
        curr_set = move(s, string(1, c));
        if (!curr_set.empty()) s = *curr_set.begin();
        else return new State();
    }
    return s;
}

void DFA::simulate(vector<string> words) {
    for (string word : words) {
        State* s = simulate(word);
        if (s->getIsAcceptingState()) {
            addToken(s, word);
        } else {
            recoveryRoutine(word);
        }
    }
}

void DFA::simulate() {simulate(this->allWords);}

vector<Token*> DFA::getTokens() {return this->Tokens;}

vector<Token*> DFA::getSymbolTable() {return this->SymbolTable;}

void DFA::addToken(State* s, string word) {
    auto t = new Token(s->getAcceptingTokenKey(), new TokenValue(word));
    this->Tokens.push_back(t);
    if (s->getAcceptingTokenKey()->getKey() == "id") {
        SymbolTable.push_back(t);
    }
    cout << '{' << t->getKey()->getKey() << ", " << t->getValue()->getValue() << "}\n";
}

void DFA::parseSrcProgram(string path) {
    fstream file;
    file.open(path,ios::in);
    if (!file.is_open()) {
        cout << "File not exist";
    }

    string line;
    while (getline(file,line)) {
        vector<string> words = split(line);
        this->allWords.insert(allWords.end(), words.begin(), words.end());
    }
    file.close();
}

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
