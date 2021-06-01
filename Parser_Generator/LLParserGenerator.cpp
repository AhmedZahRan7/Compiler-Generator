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

// {
// S -> RTx
// R -> sURb
//   -> E
// U -> uU 
// U -> E
// V -> vV
// V -> E
// T -> VtT
// T -> E
// }
bool isTerminal(Elem* e){
    return typeid(e) == typeid(Terminal); 
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

void LLParserGenerator::setFirst(NonTerminal* lhs){
    this->isFirstBuild[lhs] = true;
    Production* production  = this->rulesMapping[lhs];
    for(vector<Elem*> rhs : production->getRHS()){
        for(Elem* e : rhs){
            if(isEpsilon(e)) {
                this->first[production->getLHS()].insert((Terminal*)e);
                break;
            }
            if(isTerminal(e)){
                this->first[production->getLHS()].insert((Terminal*)e);
                break;
            }
            
            // first build my Dependance
            if(!this->isFirstBuild[(NonTerminal*)e]) setFirst((NonTerminal*)e);

            bool epsilonExist = false;
            unordered_set<Terminal*> nextFirst = getFirstOfNonTerminal((NonTerminal*)e,epsilonExist);
            for(Terminal* t : nextFirst) this->first[production->getLHS()].insert((Terminal*)e);
            if(!epsilonExist) break;

            // X -> Y1 Y2 ....... Yn -- if i reached Yn and not break then Epsilon exists in all Yi i =[1,n] then insert Epsilon in X  
            if(e == rhs.back()) this->first[production->getLHS()].insert(EPSILON);
        }
    }
}

void LLParserGenerator::buildFirst() {
    for(Production* p : this->rules){
        if(!this->isFirstBuild[p->getLHS()]) setFirst(p->getLHS());
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
                            for(Terminal* t :this->follow[rule->getLHS()]) this->follow[nt].insert(t);
                        }
                        else if(isTerminal(v[i+1])){
                            this->follow[nt].insert((Terminal*)v[i+1]);  
                        }
                        else{
                            for(Terminal* t : this->first[(NonTerminal*)v[i+1]]){
                                if(!isEpsilon(t)){
                                    this->follow[nt].insert(t);
                                }
                            }
                            bool epsilonInNext = false;
                            for(Terminal* t : this->first[(NonTerminal*)v[i+1]]) if(isEpsilon(t)) epsilonInNext = true;
                            if(epsilonInNext){
                                for(Terminal* t :this->follow[rule->getLHS()]) this->follow[nt].insert(t);
                            }
                        }
                }
            }
        }

    }
    
    /*
    S -> {  RT, BYZDF, sXd}
    R -> {Bl , E}
    Y -> lks
    Z -> 
    */
}

}
LLParser* LLParserGenerator::generateParser(){
    LLParser* parser = new LLParser();
    /**
     * @todo build this parser
     */
    return parser;
}