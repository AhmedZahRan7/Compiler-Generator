#include "NFABuilder.hpp"

NFABuilder::NFABuilder(string path){
    this->file.open(path,ios::in);
    if(!this->file) cout<<"File not exist";
    else buildTheNfa();
}

void NFABuilder::buildTheNfa(){
    while(true){
        char c = getNextChar();
        if(c==NULL) break;
        /* todo */
    }
    this->file.close();
}

char NFABuilder::getNextChar(){
    if (this->file.eof()) return NULL;
    char nextChar;
    this->file >> nextChar;
    return nextChar;
}

NFA* NFABuilder::getNFA(){
    return this->nfa;
}