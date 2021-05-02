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
}

void NFABuilder::parseRule(string& line){
    removeSpaces(line);
    cout<<line<<endl;
    RuleType type = detectRuleType(line);
    if(type == KeyWord) keyWordParser(line);
    if(type == Punctuation) punctuationParser(line);
    if(type == RegularDefinition) regularDefinitionParser(line);
    if(type == RegularExpression) regularExpressionParser(line);
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

void NFABuilder::keyWordParser(string& line){
    line.erase(0,1);
    line.erase(line.size()-1,1);
    cout<<line<<endl;
    stringstream stream(line);
    string word;
    static TokenKey * key = new TokenKey("keyWord");
    Token::addTokenKey(key);
    while (stream >> word) {
        cout<<word<<endl;
        this->nfaList.push_back(formNfaForKeyWord(word,key));
    }
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

void NFABuilder::punctuationParser(string& line){
    
}
void NFABuilder::regularDefinitionParser(string& line){
    
}
void NFABuilder::regularExpressionParser(string& line){
    
}
