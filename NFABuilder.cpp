#include "NFABuilder.hpp"

NFABuilder::NFABuilder(string path = "lexical_rules.txt"){
    this->file.open(path,ios::in);
    if(!this->file.is_open()) cout<<"File not exist";
    else buildTheNfa();
}


NFA* NFABuilder::getNFA(){
    return this->nfa;
}

void NFABuilder::buildTheNfa(){
    string line;
    while(getline(this->file,line)){
        // cout<<line<<endl;
        parseRule(line);
    }
    this->file.close();
}

void NFABuilder::parseRule(string line){
    
}
