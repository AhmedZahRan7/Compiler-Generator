#include "NFA.hpp"

NFA::NFA(){

}

NFA::NFA(string input){
    this->states.insert(this->startState = new State());
    this->states.insert(this->finalState = new State());
    this->transationsFromState[this->startState].insert(
        new Transation(this->startState,this->finalState,input));
    this->finalState->markAsAcceptingState();
}

NFA* NFA::concatinateWith(NFA* secondNFA){
    this->transationsFromState[this->finalState].insert(
        new Transation(this->finalState,secondNFA->startState,EPSILON_TRANSATION));
    for(State* state : secondNFA->states) this->states.insert(state);
    for(auto p : secondNFA->transationsFromState){
        for(Transation* trans : p.second) this->transationsFromState[p.first].insert(trans);
    }
    this->finalState->markAsNotAcceptingState();
    this->finalState = secondNFA->finalState;
    return this;
}

NFA* NFA::unionWith(NFA* secondNFA){
    State *newStart = new State();
    State *newfinal = new State();
    newfinal->markAsAcceptingState();
    this->transationsFromState[newStart].insert(new Transation(newStart,this->startState,EPSILON_TRANSATION));
    this->transationsFromState[newStart].insert(new Transation(newStart,secondNFA->startState,EPSILON_TRANSATION));
    this->transationsFromState[this->finalState].insert(new Transation(this->finalState,newfinal,EPSILON_TRANSATION));
    this->transationsFromState[secondNFA->finalState].insert(new Transation(secondNFA->finalState,newfinal,EPSILON_TRANSATION));
    this->states.insert(newStart);
    this->states.insert(newfinal);
    for(State* state : secondNFA->states) this->states.insert(state);
    for(auto p : secondNFA->transationsFromState){
        for(Transation* trans : p.second) this->transationsFromState[p.first].insert(trans);
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
    this->transationsFromState[this->finalState].insert(new Transation(this->finalState,this->startState,EPSILON_TRANSATION));
    this->transationsFromState[newStart].insert(new Transation(newStart,this->startState,EPSILON_TRANSATION));
    this->transationsFromState[newStart].insert(new Transation(newStart,newfinal,EPSILON_TRANSATION));
    this->transationsFromState[this->finalState].insert(new Transation(this->finalState,newfinal,EPSILON_TRANSATION));
    this->states.insert(newStart);
    this->states.insert(newfinal);
    this->finalState->markAsNotAcceptingState();
    this->finalState = newfinal;
    this->startState = newStart;
    return this;
}

NFA* NFA::convertIntoPostiveclosure(){
    State *newStart = new State();
    State *newfinal = new State();
    newfinal->markAsAcceptingState();
    this->transationsFromState[this->finalState].insert(new Transation(this->finalState,this->startState,EPSILON_TRANSATION));
    this->transationsFromState[newStart].insert(new Transation(newStart,this->startState,EPSILON_TRANSATION));
    this->transationsFromState[this->finalState].insert(new Transation(this->finalState,newfinal,EPSILON_TRANSATION));
    this->states.insert(newStart);
    this->states.insert(newfinal);
    this->finalState->markAsNotAcceptingState();
    this->finalState = newfinal;
    this->startState = newStart;
    return this;
}
string NFA::toString(){
    stringstream informations;
    informations << "Start state is "+this->startState->getID()+"\n";
    informations << "Final state is "+this->finalState->getID()+"\n";
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

/**
 * @todo use addState() and addTrans() in other implementations 
 */

set<State*> NFA::getStates(){
    return this->states;
}


void NFA::setStartState(State* state){
    this->startState = state;
}

State* NFA::getStartState() {return this->startState;}
State* NFA::getFinalState() {return this->finalState;}

