#include "LLParserGenerator.hpp"
LLParserGenerator::LLParserGenerator(vector<Production*> rules) {
    this->rules = rules;
    this->startState = rules[0]->getLHS();
    for(Production* p : rules) this->rulesMapping[p->getLHS()] = p;

    buildFirst();
    buildFollow();
}
//If X is a terminal symbol FIRST(X)={X}
//X is a non-terminal symbol and X -> E is a production rule then E is in FIRST(X)
//If X is a non-terminal symbol and X -> Y1 Y2..Yn is a production rule >> if terminal a in FIRST(Yi) and E is in all FIRST(Yj) for j=1,...,i-1 then a is in FIRST(X).
//if E is in all FIRST(Yj) for j=1,...,n then E is in FIRST(X).

unordered_set<Terminal*> LLParserGenerator::getFirstOfNonTerminal(NonTerminal* nt, bool& epsilonExist){
        unordered_set<Terminal*> nonTerminalFirst = this->first[nt];
        unordered_set<Terminal*> ans;
        for(Terminal* t : nonTerminalFirst){
            if(isEpsilon(t)) epsilonExist = true;
            else ans.insert(t);
        }
        return ans;
}
unordered_set<Terminal*> LLParserGenerator::getFirstOfOneProduction(vector<Elem*>& rhs){
    // if (this->firstForThisVector.find(rhs) != this->firstForThisVector.end()) return firstForThisVector[rhs];
    unordered_set<Terminal*> ans;
    for(Elem* e : rhs){
        if(isEpsilon(e)) {
            ans.insert(EPSILON);
            break;
        }
        if(isTerminal(e)){
            ans.insert((Terminal*)e);
            break;
        }
        // if this production depends on another nonTerminal
        // and the first set of this nonTerminal has not been built yet
        // then build it first
        if(!this->isFirstBuild[(NonTerminal*)e]) setFirst((NonTerminal*)e);

        bool epsilonExistInTheFollowingNonTerminal = false;
        unordered_set<Terminal*> nextFirst = getFirstOfNonTerminal((NonTerminal*)e,epsilonExistInTheFollowingNonTerminal);
        for(Terminal* t : nextFirst) ans.insert(t);
        if(!epsilonExistInTheFollowingNonTerminal) break;

        // X -> Y1 Y2 ....... Yn -- if i reached Yn and not break then Epsilon exists in all Yi i =[1,n] >> insert Epsilon in X
        if(e == rhs.back()) ans.insert(EPSILON);
    }
// firstForThisVector[rhs] =
    return  ans;
}
void LLParserGenerator::setFirst(NonTerminal* lhs){
    // cout<<"Setting First of "<<lhs->getId()<<endl;
    Production* production  = this->rulesMapping[lhs];
    for(vector<Elem*>& rhs : production->getRHS()){
        unordered_set<Terminal*> firstOfProduction = getFirstOfOneProduction(rhs);
        for(Terminal* t : firstOfProduction) this->first[lhs].insert(t);
    }
    this->isFirstBuild[lhs] = true;
}
void LLParserGenerator::buildFirst() {
    for(Production* p : this->rules){
        if(!this->isFirstBuild[p->getLHS()]) setFirst(p->getLHS());
    }
}

// If $ is the start symbol $ is in FOLLOW(S)
// if A -> xBy is a production rule >> everything in FIRST(y) is FOLLOW(B) except E
// If ( A -> xB is a production rule ) or ( A -> xBy is a production rule and E is in FIRST(y)) >> everything in FOLLOW(A) is in FOLLOW(B).

//include the follow of the LHS into this follow (non-terminal in RHS)
void LLParserGenerator::followNonTerInLast(NonTerminal* lhs , NonTerminal* rhs){
    if(lhs == rhs) return;
    if(!isFollowBuild[lhs]) setFollow(lhs);
    for(Terminal* t :this->follow[lhs]) this->follow[rhs].insert(t);
}

void LLParserGenerator::followTer(NonTerminal* nonTerminal,Terminal* terminal){
    this->follow[nonTerminal].insert(terminal);
}

void LLParserGenerator::followNonTer(NonTerminal* lhs ,NonTerminal* rhs, vector<Elem*>&rhsVector, unsigned int idx){
    for(;idx<rhsVector.size();idx++){
        Elem* next = rhsVector[idx];
        if(isTerminal(next)) {
            this->follow[rhs].insert((Terminal*) next);
            break;
        }
        bool epsilonInNext = false;
        for(Terminal* t : this->first[(NonTerminal*)next]){
            if(!isEpsilon(t)) this->follow[rhs].insert(t);
            if(isEpsilon(t)) epsilonInNext = true;
        }
        if(!epsilonInNext) break;
        //We reached the last Non-Terminal in the Production without breaking the loop
        //This means all the following set is Non-terminals with E in First
        if(idx==rhsVector.size()-1){
            if(lhs == rhs) break;
            if(!this->isFollowBuild[lhs]) this->setFollow(lhs);
            for(Terminal* t :this->follow[lhs]) this->follow[rhs].insert(t);
        }
    }
}

void LLParserGenerator::setFollow(NonTerminal* lhs){
    for(Production* rule : this->rules){
        for(vector<Elem*>&v : rule->getRHS()){
            for(unsigned int i=0;i<v.size();i++){
                // cout<<v[i]->getId()<<" ";
                if(v[i] == lhs){
                    // The non-terminal is the last element in the production
                    if(i == v.size()-1) followNonTerInLast(rule->getLHS(),lhs);
                    // The non-terminal followed by a terminal
                    else if(isTerminal(v[i+1])) followTer(lhs,(Terminal*)v[i+1]);
                    // The non-terminal followed by another non-terminal
                    else followNonTer(rule->getLHS(),lhs,v,i+1);
                }
            }
            // cout<<endl;
        }
    }
    this->isFollowBuild[lhs] = true;
}

void LLParserGenerator::buildFollow() {
    this->follow[this->getStartState()].insert(DOLLAR_SIGN);
    for(Production* p : this->rules) {
        if(!this->isFollowBuild[p->getLHS()]) setFollow(p->getLHS());
    }
}


/**
for each production rule A -> x of a grammar G
    ??? for each terminal a in FIRST(x) add A -> x to M[A,a]
    ??? If E in FIRST(x) for each terminal a in FOLLOW(A) add A -> x to M[A,a]
    ??? If E in FIRST(x) and $ in FOLLOW(A)  add A -> x to M[A,$]
 */

// x -> AB | CD   F: {a,b,c}
// X -> E
// a      b     c
// x->E  x->E   x->AB
// x->AB

LLParser* LLParserGenerator::generateParser(unordered_map<string, Terminal*> terminalsMapping) {
    LLParser* parser = new LLParser(this->startState, terminalsMapping);
    for (Production* p : this->rules) {
        bool Epsilon_Exist = false;
        NonTerminal* lhs = p->getLHS();
        for (vector<Elem*> rhs: p->getRHS()) {
            unordered_set<Terminal*> first = getFirstOfOneProduction(rhs);
            for(Terminal* fst : first) {
                if (isEpsilon(fst)) {
                    Epsilon_Exist = true;
                    unordered_set<Terminal*> follow = this->follow[lhs];
                    for (Terminal* a : follow) {
                        parser->addToTable(lhs, a, rhs);
                    }
                }
                else {
                    parser->addToTable(lhs, fst, rhs);
                }
            }
        }
        if (!Epsilon_Exist ){
           unordered_set<Terminal*> LHS_follow = follow[lhs];
            for (Terminal* a : LHS_follow) {
               if(! parser->existInTable(lhs,a) )
               parser->addToTable(lhs, a, {SYNC});
            }
        }
    }
    return parser;
}

void LLParserGenerator::printFirst(){
    cout << "First Set:\n";
    for (auto r : this->rules) {
        unordered_set<Terminal*> fst = this->first[r->getLHS()];
        cout << r->getLHS()->getId() << " ---> ";
        for (auto t : fst) {
            cout << t->getId() << " ";
        }
        cout << endl;
    }
}
void LLParserGenerator::printFollow(){
    cout << "Follow Set:\n";
    for (auto r : this->rules) {
        unordered_set<Terminal*> flw = this->follow[r->getLHS()];
        cout << r->getLHS()->getId() << " ---> ";
        for (auto t : flw) {
            cout << t->getId() << " ";
        }
        cout << endl;
    }
}
NonTerminal* LLParserGenerator::getStartState() {
    return this->startState;
}
