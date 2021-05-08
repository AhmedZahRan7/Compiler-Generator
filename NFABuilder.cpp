#include "NFABuilder.hpp"
#include "utilities.hpp"

NFABuilder::NFABuilder(string path){
    this->file.open(path,ios::in);
    if(!this->file.is_open()) cout<<"File not exist";
    else buildTheNfa();
}

NFA* NFABuilder::getNFA(){
    return mergeNfaList();
}

void NFABuilder::buildTheNfa(){
    string line;
    while(getline(this->file,line)) parseRule(line);
    this->file.close();
}

void NFABuilder::parseRule(string& line){
    if(line.size()==0) return;
    removeSpaces(line);
    RuleType type = detectRuleType(line);
    if(type == KeyWord) keyWordParser(line);
    if(type == Punctuation) punctuationParser(line);
    if(type == RegularDefinition) regularDefinitionParser(line);
    if(type == RegularExpression) regularExpressionParser(line);
}

void NFABuilder::keyWordParser(string& line){
    line.erase(0,1);
    line.erase(line.size()-1,1);
    vector<string> keyWords = split(line);
    for(string word : keyWords) {
        TokenKey* key = new TokenKey(word);
        Token::addTokenKey(key);
        this->nfaList.push_back(formNfaForKeyWord(word,key));
    }
}

void NFABuilder::punctuationParser(string& line){
    //same steps as keyWords parsing
    keyWordParser(line);
}

void NFABuilder::regularDefinitionParser(string& line){
    string name;
    string expression;
    unsigned int i=0;
    while (i<line.size()){
        if(line[i]=='=') break;
        name+=line[i];
        i++;
    }
    i++;
    while (i<line.size()){
        expression+=line[i];
        i++;
    }
    name = trim(name);
    this->regularDefinitionNFAs[name] = buildNFAFromPostfixExpression(infixToPostfix(expression));
}

void NFABuilder::regularExpressionParser(string& line){
    string name;
    string expression;
    unsigned int i=0;
    while (i<line.size()){
        if(line[i]==':') break;
        name+=line[i];
        i++;
    }
    i++;
    while (i<line.size()){
        expression+=line[i];
        i++;
    }
    trim(name);
    NFA* generatedNFA = buildNFAFromPostfixExpression(infixToPostfix(expression));
    TokenKey* key = new TokenKey(name);
    generatedNFA->getFinalState()->markAsAcceptingState();
    generatedNFA->getFinalState()->setAcceptingTokenKey(key);
    Token::addTokenKey(key);
    this->nfaList.push_back(generatedNFA);
}

NFA* NFABuilder::formNfaForKeyWord(string word,TokenKey* key){
    NFA* nfa = createNFAFromWord(word);
    nfa->getFinalState()->markAsAcceptingState();
    nfa->getFinalState()->setAcceptingTokenKey(key);
    return nfa;
}

RuleType NFABuilder::detectRuleType(string& line){
    if(line.front() == '{' && line.back() == '}') return KeyWord;
    if(line.front() == '[' && line.back() == ']') return Punctuation;
    for(char c:line){
        if(c=='=') return RegularDefinition;
        if(c==':') return RegularExpression;
    }
    cout<<"ERROR IN PARSING"<<endl;
    return KeyWord;
}

NFA* NFABuilder::createNFAFromWord(string word){
    if(word.size()==0) return NULL;
    if(word == "\\L") return new NFA(EPSILON_TRANSATION);
    word.erase(remove(word.begin(), word.end(), '\\'), word.end());
    NFA* nfa = new NFA(string(1,word[0]));
    for(unsigned int i=1;i<word.size();i++){
        char c = word[i];
        nfa = NFA::concatenate(nfa,new NFA(string(1,c)));
    }
    return nfa;
}

NFA* NFABuilder::buildNFAFromPostfixExpression(vector<string> postfix){
    stack<NFA*> accumulatedNFA;
    for(unsigned int i=0;i<postfix.size();i++){
        string word = postfix[i];
        if(word == KLEENE_CLOSURE_OPERATOR){
            NFA* nfa = NFA::convertIntoKleeneClosure(accumulatedNFA.top());
            accumulatedNFA.pop();
            accumulatedNFA.push(nfa);
        }
        else if(word == POSITIVE_CLOSURE_OPERATOR){
            NFA* nfa = NFA::convertIntoPositiveClosure(accumulatedNFA.top());
            accumulatedNFA.pop();
            accumulatedNFA.push(nfa);
        }
        else if(word == UNION_OPERATOR){
            NFA* topNfa1 = accumulatedNFA.top();
            accumulatedNFA.pop();
            NFA* topNfa2 = accumulatedNFA.top();
            accumulatedNFA.pop();
            accumulatedNFA.push(NFA::unionWith(topNfa2,topNfa1));
        }
        else if(word == CONCATENATION_OPERATOR){
            NFA* topNfa1 = accumulatedNFA.top();
            accumulatedNFA.pop();
            NFA* topNfa2 = accumulatedNFA.top();
            accumulatedNFA.pop();
            accumulatedNFA.push(NFA::concatenate(topNfa2,topNfa1));
        }
        else if(word == RANGE_OPERATOR){
            accumulatedNFA.pop();
            accumulatedNFA.pop();
            string from = postfix[i-2];
            string to = postfix[i-1];
            if(from.size()!=1 || to.size()!=1 ) cout<<"RANGE OPERATOR IS DONE FOR MORE THAN ONE CHAR"<<endl;
            accumulatedNFA.push(NFA::formNFAForRangeOperator(from[0],to[0]));
        }
        else if(this->regularDefinitionNFAs.count(word)) accumulatedNFA.push(regularDefinitionNFAs[word]);
        else accumulatedNFA.push(createNFAFromWord(word));
    }
    if(accumulatedNFA.size()!=1) cout<<"END ACCUMULATION WITH "<<accumulatedNFA.size()<<" NFAs IN THE STACK"<<endl;
    return accumulatedNFA.top();
}

NFA* NFABuilder::mergeNfaList(){
    NFA* nfa = new NFA();
    State* startState = new State();
    nfa->addState(startState);
    for(NFA* addNFA : this->nfaList){
        nfa->addTransation(startState,addNFA->getStartState(),EPSILON_TRANSATION);
        for(State* state : addNFA->getStates()){
            nfa->addState(state);
            for(Transation* t : addNFA->getTransationFromState(state)) nfa->addTransation(state,t);
        }
    }
    nfa->setStartState(startState);
    return nfa;
}
