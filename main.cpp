#include <bits/stdc++.h>
#include "Lexical_Analyzer_Generator.hpp"
#include "Lexical_Analyzer.hpp"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Number of arguments is invalid\nFormat is <rules file path> <src program path>\n";
        return 1;
    } else {
        auto lex_gen = new Lexical_Analyzer_Generator(argv[1]);
        auto lex = lex_gen->get_Lexical_Analyzer();
        lex->simulateProgramFile(argv[2]);
        Token* t;
        while ((t = lex->getToken()) != nullptr) {
            cout << '{' << t->getKey()->getKey() << ", " << t->getValue()->getValue() << "}\n";
        }
        return 0;
    }
}
