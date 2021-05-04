#include "NFA.hpp"

NFA::NFA(){

}

NFA::NFA(string input){
    addState(this->startState = new State());
    addState(this->finalState = new State());
    addTransation(this->startState,this->finalState,input);
    this->finalState->markAsAcceptingState();
}

NFA* NFA::concatinateWith(NFA* secondNFA){
    addTransation(this->finalState,secondNFA->startState,EPSILON_TRANSATION);
    for(State* state : secondNFA->states) addState(state);
    for(auto p : secondNFA->transationsFromState){
        for(Transation* trans : p.second)
            addTransation(trans->from,trans->to,trans->condition);
    }
    this->finalState->markAsNotAcceptingState();
    this->finalState = secondNFA->finalState;
    return this;
}

NFA* NFA::unionWith(NFA* secondNFA){
    State *newStart = new State();
    State *newfinal = new State();
    newfinal->markAsAcceptingState();
    addTransation(newStart,this->startState,EPSILON_TRANSATION);
    addTransation(newStart,secondNFA->startState,EPSILON_TRANSATION);
    addTransation(this->finalState,newfinal,EPSILON_TRANSATION);
    addTransation(secondNFA->finalState,newfinal,EPSILON_TRANSATION);
    addState(newStart);
    addState(newfinal);
    for(State* state : secondNFA->states) addState(state);
    for(auto p : secondNFA->transationsFromState){
        for(Transation* trans : p.second)
            addTransation(trans->from,trans->to,trans->condition);
    }
    this->finalState->markAsNotAcceptingState();
    this->finalState = newfinal;
    this->startState = newStart;
    return this;
}

NFA* NFA::convertIntoKleeneclosure(){
    State *newStart = new State();
    State *newfinal = new State();
    newfinal->markAsAcceptingState();
    addTransation(this->finalState,this->startState,EPSILON_TRANSATION);
    addTransation(this->finalState,newfinal,EPSILON_TRANSATION);
    addTransation(newStart,this->startState,EPSILON_TRANSATION);
    addTransation(newStart,newfinal,EPSILON_TRANSATION);
    addState(newStart);
    addState(newfinal);
    this->finalState->markAsNotAcceptingState();
    this->finalState = newfinal;
    this->startState = newStart;
    return this;
}

NFA* NFA::convertIntoPostiveclosure(){
    State *newStart = new State();
    State *newfinal = new State();
    newfinal->markAsAcceptingState();
    addTransation(this->finalState,this->startState,EPSILON_TRANSATION);
    addTransation(this->finalState,newfinal,EPSILON_TRANSATION);
    addTransation(newStart,this->startState,EPSILON_TRANSATION);
    addState(newStart);
    addState(newfinal);
    this->finalState->markAsNotAcceptingState();
    this->finalState = newfinal;
    this->startState = newStart;
    return this;
}
string NFA::toString(){
    stringstream informations;
    informations << "Start state is "+this->startState->getID()+"\n";
    // informations << "Final state is "+this->finalState->getID()+"\n";
    for(auto p : this->transationsFromState){
        informations<<p.first->getID()<<"  \n";
        for(Transation* trans : p.second) informations<<"--"<<trans->condition<<"-->"<<trans->to->getID()<<"  \n";
    }
    return informations.str();
}


void NFA::addState(State* state){
    this->states.insert(state);
}
void NFA::addTransation(State* from,State* to,string cond){
    this->transationsFromState[from].insert(new Transation(from,to,cond));
}

set<State*> NFA::getStates(){
    return this->states;
}


void NFA::setStartState(State* state){
    this->startState = state;
}

set<Transation*> NFA::getTransationFromState(State* state){
    return this->transationsFromState[state];
}
State* NFA::getStartState() {return this->startState;}
State* NFA::getFinalState() {return this->finalState;}

