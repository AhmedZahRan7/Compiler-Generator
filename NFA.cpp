#include "NFA.hpp"
NFA::NFA(){

}
NFA::NFA(string input){
    addState(this->startState = new State());
    addState(this->finalState = new State());
    addTransation(this->startState,this->finalState,input);
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
    NFA* nfa1 = firstNFA->clone();
    NFA* nfa2 = secondNFA->clone();
    nfa1->states.erase(nfa1->finalState);
    for(State* state:nfa1->states){
        for(Transation* t : nfa1->getTransationFromState(state)){
            if(t->to == nfa1->getFinalState()) t->to = nfa2->startState;
        }
    }
    for(State* state : nfa2->states){
        nfa1->addState(state);
        for(Transation* trans : nfa2->getTransationFromState(state)) nfa1->addTransation(state,trans);
    }
    nfa1->setFinalState(nfa2->finalState);
    return nfa1;
}
NFA* NFA::unionWith(NFA* firstNFA,NFA* secondNFA){
    NFA* nfa1 = firstNFA->clone();
    NFA* nfa2 = secondNFA->clone();
    for(State* state : nfa2->states){
        nfa1->addState(state);
        for(Transation* trans : nfa2->getTransationFromState(state)) nfa1->addTransation(state,trans);
    }
    State* newStart = new State();
    State* newFinal = new State();
    nfa1->addTransation(newStart,nfa1->startState,EPSILON_TRANSATION);
    nfa1->addTransation(newStart,nfa2->startState,EPSILON_TRANSATION);
    nfa1->addTransation(nfa1->finalState,newFinal,EPSILON_TRANSATION);
    nfa1->addTransation(nfa2->finalState,newFinal,EPSILON_TRANSATION);
    nfa1->setStartState(newStart);
    nfa1->setFinalState(newFinal);
    nfa1->addState(newStart);
    nfa1->addState(newFinal);
    return nfa1;
}
NFA* NFA::convertIntoKleeneClosure(NFA* oldNfa){
    NFA* nfa = convertIntoPositiveClosure(oldNfa);
    nfa->addTransation(nfa->startState,nfa->finalState,EPSILON_TRANSATION);
    return nfa;
}
NFA* NFA::convertIntoPositiveClosure(NFA* oldNfa){
    NFA * nfa = oldNfa->clone();
    State* newStart = new State();
    State* newFinal = new State();
    nfa->addState(newStart);
    nfa->addState(newFinal);
    nfa->addTransation(nfa->finalState,nfa->startState,EPSILON_TRANSATION);
    nfa->addTransation(newStart,nfa->startState,EPSILON_TRANSATION);
    nfa->addTransation(nfa->finalState,newFinal,EPSILON_TRANSATION);
    nfa->setStartState(newStart);
    nfa->setFinalState(newFinal);
    return nfa;
}
NFA* NFA::formNFAForRangeOperator(char from,char to){
    NFA* nfa = new NFA();
    nfa->setStartState(new State());
    nfa->setFinalState(new State());
    nfa->addState(nfa->getStartState());
    nfa->addState(nfa->getFinalState());
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
    for(State* state : this->states){
        informations<<state->getID()<<"  \n";
        for(Transation* trans : getTransationFromState(state)) {
            informations<<"--"<<trans->condition<<"-->"<<trans->to->getID();
            if(trans->to->getIsAcceptingState()) informations<<" ("<<trans->to->getAcceptingTokenKey()->getKey()<<")";
            informations<<"  \n";
        }
    }
    return informations.str();
}
map<State*,set<Transation*>> NFA::getTransitionTable() {return this->transationsFromState;}
set<string> NFA::getInputSymbols() {
    set<string> inputSymbols;
    for(auto p : this->transationsFromState)
        for(Transation* trans : p.second)
            if(trans->condition != EPSILON_TRANSATION) inputSymbols.insert(trans->condition);
    return inputSymbols;
}
NFA* NFA::clone(){
    NFA* nfa = new NFA();
    unordered_map<State*,State*> maping;
    for(State* state : this->states) maping[state] = state->clone();
    for(State* state : this->states){
        nfa->addState(maping[state]);
        for(Transation* trans : this->getTransationFromState(state))
            nfa->addTransation(maping[trans->from],maping[trans->to],trans->condition);
    }
    nfa->setStartState(maping[this->startState]);
    nfa->setFinalState(maping[this->finalState]);
    return nfa;
}
