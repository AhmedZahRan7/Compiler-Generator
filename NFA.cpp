#include "NFA.hpp"
NFA::NFA(){
    addState(this->startState = new State());
    addState(this->finalState = new State());
    addTransation(this->startState,this->finalState,EPSILON_TRANSATION);
}
NFA::NFA(string input){
    addState(this->startState = new State());
    addState(this->finalState = new State());
    addTransation(this->startState,this->finalState,input);
    this->finalState->markAsAcceptingState();
}
void NFA::addState(State* state){
    this->states.insert(state);
}
void NFA::addTransation(State* from,State* to,string cond){
    this->transationsFromState[from].insert(new Transation(from,to,cond));
}
void NFA::addTransation(State* from,Transation* trans){
    this->transationsFromState[from].insert(trans);
}
set<State*> NFA::getStates(){
    return this->states;
}
void NFA::setStartState(State* state){
    this->startState = state;
}
void NFA::setFinalState(State* state){
    this->finalState = state;
}
set<Transation*> NFA::getTransationFromState(State* state){
    return this->transationsFromState[state];
}
State* NFA::getStartState() {
    return this->startState;
}
State* NFA::getFinalState() {

    return this->finalState;
}
NFA* NFA::concatenate(NFA* firstNFA,NFA* secondNFA){
    NFA * nfa = new NFA();
    nfa->setStartState(firstNFA->getStartState());
    nfa->setFinalState(secondNFA->getFinalState());
    for(State* state : firstNFA->states){
        if(state != firstNFA->finalState) nfa->addState(state);
        for(Transation* trans : firstNFA->getTransationFromState(state)){
            if(trans->to != firstNFA->finalState) nfa->addTransation(state,trans);
            else nfa->addTransation(state,secondNFA->startState,trans->condition);
        }
    }
    for(State* state : secondNFA->states){
        nfa->addState(state);
        for(Transation* trans : secondNFA->getTransationFromState(state)) nfa->addTransation(state,trans);
    }
    return nfa;
}
NFA* NFA::unionWith(NFA* firstNFA,NFA* secondNFA){
    NFA * nfa = new NFA();
    nfa->setStartState(new State());
    nfa->setFinalState(new State());
    nfa->addState(nfa->startState);
    nfa->addState(nfa->finalState);
    nfa->getFinalState()->markAsAcceptingState();

    State* firstNFAFinal = new State();
    State* secondNFAFinal = new State();

    for(State* state : firstNFA->states){
        if(state != firstNFA->finalState) nfa->addState(state);
        else nfa->addState(firstNFAFinal);
        for(Transation* trans : firstNFA->getTransationFromState(state)){
            if(trans->to != firstNFA->finalState) nfa->addTransation(state,trans);
            else nfa->addTransation(state,firstNFAFinal,trans->condition);
        }
    }
    for(State* state : secondNFA->states){
        if(state != secondNFA->finalState) nfa->addState(state);
        else nfa->addState(secondNFAFinal);
        for(Transation* trans : secondNFA->getTransationFromState(state)){
            if(trans->to != secondNFA->finalState) nfa->addTransation(state,trans);
            else nfa->addTransation(state,secondNFAFinal,trans->condition);
        }
    }
    nfa->addTransation(nfa->startState,firstNFA->startState,EPSILON_TRANSATION);
    nfa->addTransation(nfa->startState,secondNFA->startState,EPSILON_TRANSATION);
    nfa->addTransation(firstNFAFinal,nfa->finalState,EPSILON_TRANSATION);
    nfa->addTransation(secondNFAFinal,nfa->finalState,EPSILON_TRANSATION);
    return nfa;
}
NFA* NFA::convertIntoKleeneClosure(NFA* oldNfa){
    NFA* nfa = convertIntoPositiveClosure(oldNfa);
    nfa->addTransation(nfa->startState,nfa->finalState,EPSILON_TRANSATION);
    return nfa;
}
NFA* NFA::convertIntoPositiveClosure(NFA* oldNfa){
    NFA * nfa = new NFA();
    nfa->setStartState(new State());
    nfa->setFinalState(new State());
    nfa->getFinalState()->markAsAcceptingState();
    State* newFinal = new State();
    for(State* state : oldNfa->states){
        if(state != oldNfa->finalState) nfa->addState(state);
        else nfa->addState(newFinal);
        for(Transation* trans : oldNfa->getTransationFromState(state)){
            if(trans->to != oldNfa->finalState) nfa->addTransation(state,trans);
            else nfa->addTransation(state,newFinal,trans->condition);
        }
    }
    nfa->addTransation(newFinal,oldNfa->startState,EPSILON_TRANSATION);
    nfa->addTransation(newFinal,nfa->finalState,EPSILON_TRANSATION);
    nfa->addTransation(nfa->startState,oldNfa->startState,EPSILON_TRANSATION);
    return nfa;
}
NFA* NFA::formNFAForRangeOperator(char from,char to){
    NFA* nfa = new NFA();
    nfa->setStartState(new State());
    nfa->setFinalState(new State());
    nfa->getFinalState()->markAsAcceptingState();
    for(char c=from;c<=to;c++){
        NFA* subNFA = new NFA(string(1,c));
        nfa->addTransation(nfa->startState,subNFA->startState,EPSILON_TRANSATION);
        nfa->addTransation(subNFA->finalState,nfa->finalState,EPSILON_TRANSATION);
        for(State* state : subNFA->states){
            nfa->addState(state);
            for(Transation* trans : subNFA->getTransationFromState(state)) nfa->addTransation(state,trans);
        }
    }
    return nfa;
}
string NFA::toString(){
    stringstream informations;
    informations << "Start state is "+this->startState->getID()+"\n";
    if(finalState!=NULL)informations << "Final state is "+this->finalState->getID()+"\n";
    for(auto p : this->transationsFromState){
        informations<<p.first->getID()<<"  \n";
        for(Transation* trans : p.second) {
            informations<<"--"<<trans->condition<<"-->"<<trans->to->getID();
            if(trans->to->getIsAcceptingState()) informations<<" ("<<trans->to->getAcceptingTokenKey()->getKey()<<")";
            informations<<"  \n";
        }
    }
    return informations.str();
}
map<State*,set<Transation*>> NFA::getTransitionTable() {return this->transationsFromState;}