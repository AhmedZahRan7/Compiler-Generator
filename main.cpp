#include <bits/stdc++.h>
#include "Lexical_Analyzer_Generator.hpp"
#include "LLParserGenerator.hpp"
#include "CFGBuilder.hpp"
#include "CFG.hpp"
#include "Lexical_Analyzer.hpp"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Number of arguments is invalid\nFormat is <lex rules file> <parser rules file> <src program>\n";
        return 1;
    } else {
        fstream lexRules, parserRules, srcProgram;
        lexRules.open(argv[1], ios::in);
        parserRules.open(argv[2], ios::in);
        srcProgram.open(argv[3], ios::in);
        if (!lexRules.is_open() || !parserRules.is_open() || !srcProgram.is_open()) {
            cout << "A file of the three does not exist\n";
            return 1;
        }
        lexRules.close();
        parserRules.close();

        auto lex_gen = new Lexical_Analyzer_Generator(argv[1]);
        auto lex = lex_gen->get_Lexical_Analyzer();

        auto cfgBuilder = new CFGBuilder(argv[2], *lex->getOrderedKeys());
        auto cfg = new CFG(cfgBuilder->getProcs());
        auto generator = new LLParserGenerator(cfg->getProcs());
        auto parser = generator->generateParser(cfgBuilder->getTerminals());

        string line;
        while (getline(srcProgram,line)) {
            vector<string> words = split(line);
            for (auto w : words) {
                lex->simulate(w);
                Token* t;
                while ((t = lex->getNextToken()) != nullptr) {
                    parser->parse(t);
                }
            }
        }
        srcProgram.close();
        Token* dollar_sign = new Token(new TokenKey("$"), new TokenValue("$"));
        parser->parse(dollar_sign);
        
        return 0;
    }
}
