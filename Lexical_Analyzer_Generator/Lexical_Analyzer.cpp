#include "Lexical_Analyzer.hpp"

Lexical_Analyzer::Lexical_Analyzer(set<State*> DStates, map<State*, set<Transation*>> Dtransitions, State* startState) {
    this->DStates = DStates;
    this->Dtransitions = Dtransitions;
    this->startState = startState;
}

State* Lexical_Analyzer::simNormal(string word) {
    State* s = this->startState;
    set<State*> curr_set;
    for (char c : word) {
        curr_set = move(s, string(1, c));
        if (!curr_set.empty()) s = *curr_set.begin();
        else return new State();
    }
    return s;
}

void Lexical_Analyzer::simulate(string word) {
    State* s = simNormal(word);
    if (s->getIsAcceptingState()) {
        addToken(s, word);
    } else {
        simRecovery(word);
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
    this->tokenBuffer.push(t);
    if (s->getAcceptingTokenKey()->getKey() == "id") {
        SymbolTable.push_back(t);
    }
}

void Lexical_Analyzer::simRecovery(string word) {
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

Token* Lexical_Analyzer::getNextToken() {
    if (!tokenBuffer.empty()) {
        Token* t = tokenBuffer.front();
        tokenBuffer.pop();
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
