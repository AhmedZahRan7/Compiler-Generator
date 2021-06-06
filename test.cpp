#include "utilities.hpp"
#include "CFGBuilder.hpp"
#include "CFG.hpp"
#include "Lexical_Analyzer_Generator.hpp"
#include "Parser_Generator/LLParserGenerator.hpp"


int main() {
    auto lex_gen = new Lexical_Analyzer_Generator("Lexical_Analyzer_Generator\\lab_rules.txt");
    auto lex = lex_gen->get_Lexical_Analyzer();

    CFGBuilder* cfgBuilder = new CFGBuilder("Parser_Generator\\test_rules_file.txt", *lex->getOrderedKeys());
    cfgBuilder->printProductions();
    cout << endl;
    CFG* cfg = new CFG(cfgBuilder->getProcs());
    for (auto p : cfg->getProcs()) cout << p->toString();
    // LLParserGenerator* generator =  new LLParserGenerator(cfgBuilder->getProcs());

    // generator->printFirst();
    // cout << endl;
    // generator->printFollow();

    // fstream file;
    // file.open("lab_program.txt", ios::in);
    // if (!file.is_open()) {
    //     cout << "File not exist";
    //     return 0;
    // }
    // auto parser = generator->generateParser(cfg->getTerminals());
    // string line;
    // cout << "\nStart Parsing:\n";
    // while (getline(file,line)) {
    //     vector<string> words = split(line);
    //     for (auto w : words) {
    //         lex->simulate(w);
    //         Token* t;
    //         while ((t = lex->getNextToken()) != nullptr) {
    //             parser->parse(t);
    //         }
    //     }
    // }
    // file.close();
    // Token* dollar_sign = new Token(new TokenKey("$"), new TokenValue("$"));
    // parser->parse(dollar_sign);

    return 0;
}
