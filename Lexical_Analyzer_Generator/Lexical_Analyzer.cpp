#include "Lexical_Analyzer.hpp"

Lexical_Analyzer::Lexical_Analyzer(set<State*> DStates, map<State*, set<Transation*>> Dtransitions, State* startState) {
    this->DStates = DStates;
    this->Dtransitions = Dtransitions;
    this->startState = startState;
}

State* Lexical_Analyzer::simulate(string word) {
    State* s = this->startState;
    set<State*> curr_set;
    for (char c : word) {
        curr_set = move(s, string(1, c));
        if (!curr_set.empty()) s = *curr_set.begin();
        else return new State();
    }
    return s;
}

void Lexical_Analyzer::simulate(vector<string> words) {
    for (string word : words) {
        State* s = simulate(word);
        if (s->getIsAcceptingState()) {
            addToken(s, word);
        } else {
            recoveryRoutine(word);
        }
    }
}

set<State*> Lexical_Analyzer::move(State* s, string a) {
    set<State*> nextStates;
    set<Transation*> transitions = this->Dtransitions[s];
    for (auto t : transitions) {
        if (t->condition == a) {
            nextStates.insert(t->to);
        }
    }
    return nextStates;
}

void Lexical_Analyzer::addToken(State* s, string word) {
    auto t = new Token(s->getAcceptingTokenKey(), new TokenValue(word));
    this->Tokens.push_back(t);
    if (s->getAcceptingTokenKey()->getKey() == "id") {
        SymbolTable.push_back(t);
    }
}

void Lexical_Analyzer::recoveryRoutine(string word) {
    State* s = this->startState;
    set<State*> curr_set;
    string buffer;
    pair<State*, int> lastAccept;
    bool wordFinished = false;
    while (!wordFinished) {
        for (unsigned int i = 0; i < word.size() && !word.empty(); ++i) {
            buffer += word[i];
            curr_set = move(s, string(1, word[i]));
            if (curr_set.empty()) {
                if (lastAccept.second == 0) {    // No accepting state is found til now
                    cout << word << " is not identified so neglected.\n";
                    return; // If input prefix doesn't move to any state then the rest of the word neglected
                } else {
                    buffer = "";
                    addToken(s, word.substr(0, lastAccept.second));
                    word.erase(0, lastAccept.second);
                    lastAccept = {};
                    i = -1; // Will be incremented in the for loop
                }
                s = this->startState;
            } else {
                s = *curr_set.begin();
                if (s->getIsAcceptingState()) {
                    lastAccept = {s, i + 1};
                }
            }
        }
        if (lastAccept.second == 0) {
            if (!buffer.empty()) cout << buffer << " is not identified so neglected.\n";
            wordFinished = true;
        } else {
            buffer = "";
            addToken(s, word.substr(0, lastAccept.second));
            word.erase(0, lastAccept.second);
            lastAccept = {};
        }
    }
}

void Lexical_Analyzer::simulateProgramFile(string path) {
    fstream file;
    file.open(path, ios::in);
    if (!file.is_open()) {
        cout << "File not exist";
    }

    string line;
    while (getline(file,line)) {
        vector<string> words = split(line);
        simulate(words);
    }
    file.close();
}

Token* Lexical_Analyzer::getToken() {
    if (!Tokens.empty() && token_index < Tokens.size()) {
        Token* t = this->Tokens[token_index++];
        return t;
    }
    return nullptr;
}

vector<Token*> Lexical_Analyzer::getSymbolTable() {return this->SymbolTable;}

string Lexical_Analyzer::printTransitionTable() {
    stringstream out;
    out << "Start state is "+ this->startState->getID() + "\n";
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

set<TokenKey*>* Lexical_Analyzer::getOrderedKeys() {
    return Token::getOrderedKeys();
}
