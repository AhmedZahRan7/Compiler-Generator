#include "LLParserGenerator.hpp"
LLParserGenerator::LLParserGenerator(vector<Production*> rules,set<Terminal*> allTerminals){
    this->rules = rules;
    this->allTerminals = allTerminals;
    for(Production* p : rules) this->rulesMapping[p->getLHS()] = p;
    buildFirst();
    buildFollow();
}
//If X is a terminal symbol FIRST(X)={X}
//X is a non-terminal symbol and X -> E is a production rule then E is in FIRST(X)
//If X is a non-terminal symbol and X -> Y1 Y2..Yn is a production rule >> if terminal a in FIRST(Yi) and E is in all FIRST(Yj) for j=1,...,i-1 then a is in FIRST(X).
//if E is in all FIRST(Yj) for j=1,...,n then E is in FIRST(X).
bool isTerminal(Elem* e){
    return dynamic_cast<Terminal*>(e) != nullptr;
}
bool isEpsilon(Elem* e){
    return e == EPSILON;
}
unordered_set<Terminal*> LLParserGenerator::getFirstOfNonTerminal(NonTerminal* nt, bool& epsilonExist){
        unordered_set<Terminal*> nonTerminalFirst = this->first[nt];
        unordered_set<Terminal*> ans;
        for(Terminal* t : nonTerminalFirst){
            if(isEpsilon(t)) epsilonExist = true;
            else ans.insert(t);
        }
        return ans;
}
unordered_set<Terminal*> LLParserGenerator::getFirstOfOneProduction(vector<Elem*> rhs){

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
    return ans;
}
void LLParserGenerator::setFirst(NonTerminal* lhs){
    Production* production  = this->rulesMapping[lhs];
    for(vector<Elem*> rhs : production->getRHS()){
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
void LLParserGenerator::printFirst(){
    for(auto p : this->first){
        cout<< p.first->getId();
        cout<<" -> ";
        for(Elem* e:p.second) cout << e->getId() << " ";
        cout<<endl;
    }
}
void LLParserGenerator::printFollow(){
    for(auto p : this->follow){
        cout<< p.first->getId();
        cout<<" -> ";
        for(Elem* e:p.second) cout << e->getId() << " ";
        cout<<endl;
    }
}
// If $ is the start symbol $ is in FOLLOW(S)
// if A -> xBy is a production rule >> everything in FIRST(y) is FOLLOW(B) except E
// If ( A -> xB is a production rule ) or ( A -> xBy is a production rule and E is in FIRST(y)) >> everything in FOLLOW(A) is in FOLLOW(B).
void LLParserGenerator::buildFollow() {
    this->follow[this->rules[0]->getLHS()].insert(DOLLAR_SIGN);
    for(Production* p : this->rules){
        NonTerminal * nt = p->getLHS();
        for(Production* rule : this->rules){
            for( vector<Elem*> v : rule->getRHS()){
                for(int i=0;i<v.size();i++){
                    if(v[i] == nt){
                        if(i == v.size()-1){
                            followNonTerInLast(rule->getLHS(),nt);
                        }
                        else if(isTerminal(v[i+1])){
                            followTer(nt,(Terminal*)v[i+1]);
                        }
                        else{
                            followNonTer(rule->getLHS(),nt,(NonTerminal*)v[i+1]);
                        }
                    }
                }
            }
        }
    }
}
void LLParserGenerator::followNonTerInLast(NonTerminal* lhs , NonTerminal* rhs){
    for(Terminal* t :this->follow[lhs]) this->follow[rhs].insert(t);
}
void LLParserGenerator::followTer(NonTerminal* nonTerminal,Terminal* terminal){
    this->follow[nonTerminal].insert(terminal);
}
void LLParserGenerator::followNonTer(NonTerminal* lhs ,NonTerminal* rhs, NonTerminal* nextOfRhs){
    bool epsilonInNext = false;
    for(Terminal* t : this->first[nextOfRhs]){
        if(!isEpsilon(t)) this->follow[rhs].insert(t);
        if(isEpsilon(t)) epsilonInNext = true;
    }
    if(epsilonInNext){
        for(Terminal* t :this->follow[lhs]) this->follow[rhs].insert(t);
    }
}
/**
for each production rule A -> x of a grammar G
    – for each terminal a in FIRST(x) add A -> x to M[A,a]
    – If E in FIRST(x) for each terminal a in FOLLOW(A) add A -> x to M[A,a]
    – If E in FIRST(x) and $ in FOLLOW(A)  add A -> x to M[A,$]
 */
LLParser* LLParserGenerator::generateParser(){
    LLParser* parser = new LLParser();
    // unordered_map<Terminal*,vector<Elem*> >> parsingTable ;
    // for(Production* p : this->rules){
    //     NonTerminal * nt = p->getLHS();
    //     for(vector<Elem*>rhs: p->getRHS()){
    //         vector<Elem*>first = setFirst(rhs);
    //             for(Elem* fst : first){
    //                 if(isEpsilon(fst)){
    //                     vector<Elem*>follow = buildFollow(rhs);
    //                     for(Elem* a : follow){
    //                         parsingTable[lhs][a]=rhs;
    //                     }
    //                 }else{
    //                     parsingTable[lhs][fst]=rhs;
    //                     }
    //                 }
    //             }

    //     }

    // }
    return parser;
}

NonTerminal* LLParserGenerator::getStartState() {return rules[0]->getLHS();}
