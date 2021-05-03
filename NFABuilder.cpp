#include "NFABuilder.hpp"
#include "utilities.hpp"

NFABuilder::NFABuilder(string path){
    this->file.open(path,ios::in);
    if(!this->file.is_open()) cout<<"File not exist";
    else buildTheNfa();
}

NFA* NFABuilder::getNFA(){
    return this->nfa;
}

void NFABuilder::buildTheNfa(){
    string line;
    while(getline(this->file,line)) parseRule(line);
    this->file.close();
    mergeNfaList();
}

void NFABuilder::parseRule(string& line){
    removeSpaces(line);
    // cout<<line<<endl;
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
        if(word[0]=='\\') word.erase(0,1);
        cout<<word<<endl;
        TokenKey* key = new TokenKey(word);
        Token::addTokenKey(key);
        this->nfaList.push_back(formNfaForKeyWord(word,key));
    }
}

void NFABuilder::punctuationParser(string& line){
    //same steps as keyWords parsing
    keyWordParser(line);
}

NFA* NFABuilder::formNfaForKeyWord(string word,TokenKey* key){
    if(word.size()==0) return NULL;
    NFA* nfa = new NFA(to_string(word[0]));
    for(int i=1;i<word.size();i++) nfa->concatinateWith(new NFA(to_string(word[i])));
    nfa->getFinalState()->setAcceptingTokenKey(key);
    return nfa;
}

RuleType NFABuilder::detectRuleType(string& line){
    if(line.front() == '{' && line.back() == '}') return KeyWord;
    if(line.front() == '[' && line.back() == ']') return Punctuation;
    for(char c:line){
        if(c==':') return RegularDefinition;
        if(c=='=') return RegularExpression;
    }
    cout<<"ERROR IN PARSING"<<endl;
    return KeyWord;
}

NFA* NFABuilder::buildNFAFromPostfixExpression(vector<string> postfix){
    stack<NFA*> accumulatedNFA;
    for(int i=0;i<postfix.size();i++){
        string word = postfix[i];
        if(word == KLEENE_CLOSURE_OPERATOR) accumulatedNFA.top()->convertIntoKleeneclosure();
        else if(word == POSTIVE_CLOSURE_OPERATOR) accumulatedNFA.top()->convertIntoPostiveclosure();
        else if(word == UNION_OPERATOR){
            NFA* topNfa = accumulatedNFA.top();
            accumulatedNFA.pop();
            accumulatedNFA.top()->unionWith(topNfa); 
        }
        else if(word == CONCATIONATION_OPERATOR){
            NFA* topNfa = accumulatedNFA.top();
            accumulatedNFA.pop();
            accumulatedNFA.top()->concatinateWith(topNfa); 
        }
        else if(word == RANGE_OPERATOR){
            accumulatedNFA.pop();
            accumulatedNFA.pop();
            string from = postfix[i-2];
            string to = postfix[i-1];
            NFA* nfa = new NFA(from);
            for(from[0]=from[0]+1;from[0]<=to[0];from[0]++) nfa->unionWith(new NFA(from));
            accumulatedNFA.push(nfa);
        }
        else if(this->regularDefinitionNFAs.count(word)) accumulatedNFA.push(regularDefinitionNFAs[word]);
        else accumulatedNFA.push(new NFA(word));
    }
    if(accumulatedNFA.size()!=1) cout<<"END ACCUMULATION WITH "<<accumulatedNFA.size()<<" NFAs IN THE STACK"<<endl;
    return accumulatedNFA.top();
}

void NFABuilder::regularDefinitionParser(string& line){
    string name;
    string expression;
    int i=0;
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
    this->regularDefinitionNFAs[name] = buildNFAFromPostfixExpression(infixToPostfix(expression));
}

void NFABuilder::regularExpressionParser(string& line){
    string name;
    string expression;
    int i=0;
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
    NFA* generatedNFA = buildNFAFromPostfixExpression(infixToPostfix(expression));
    TokenKey* key = new TokenKey(name);
    generatedNFA->getFinalState()->setAcceptingTokenKey(key);
    Token().addTokenKey(key);
    this->nfaList.push_back(generatedNFA);
}

void NFABuilder::mergeNfaList(){
    this->nfa = new NFA();
    State * startState = new State();
    for(NFA * addNFA : this->nfaList){
        this->nfa->addTransation(startState,addNFA->getStartState(),EPSILON_TRANSATION);
        for(State* state : addNFA->getStates()) this->nfa->addState(state);
    }
    this->nfa->setStartState(startState);
}
