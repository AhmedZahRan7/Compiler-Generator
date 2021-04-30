#include "NFA.hpp"

NFA::NFA(string input){
    this->states.insert(this->startState = new State());
    this->states.insert(this->finalState = new State());
    this->transationsFromState[this->startState].insert(
        new Transation(this->startState,this->finalState,input));
    this->finalState->markAsAcceptingState();
}

void NFA::concatinateWith(NFA* secondNFA){
    this->transationsFromState[this->finalState].insert(
        new Transation(this->finalState,secondNFA->startState,EPSILON_TRANSATION));
    for(State* state : secondNFA->states) this->states.insert(state);
    for(auto p : secondNFA->transationsFromState){
        for(Transation* trans : p.second) this->transationsFromState[p.first].insert(trans);
    }
    this->finalState->markAsNotAcceptingState();
    this->finalState = secondNFA->finalState;
}

string NFA::toString(){
    stringstream informations;
    informations << "Strat state is "+this->startState->getID()+"\n";
    informations << "Final state is "+this->finalState->getID()+"\n";
    for(auto p : this->transationsFromState){
        informations<<p.first->getID()<<"  \n";
        for(Transation* trans : p.second) informations<<"--"<<trans->condition<<"-->"<<trans->to->getID()<<"  \n";
    }
    return informations.str();
}

State* NFA::getStartState() {return this->startState;}