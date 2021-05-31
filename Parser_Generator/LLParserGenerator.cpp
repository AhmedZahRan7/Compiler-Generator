#include "LLParserGenerator.hpp"

LLParserGenerator::LLParserGenerator(vector<Production*> rules,set<Terminal*> allTerminals){
    this->rules = rules;
    this->allTerminals = allTerminals;
    buildFirst();
    buildFollow();
}
void LLParserGenerator::buildFirst() {

}
void LLParserGenerator::buildFollow() {

}
LLParser* LLParserGenerator::generateParser(){
    LLParser* parser = new LLParser();
    /**
     * @todo build this parser
     */
    return parser;
}