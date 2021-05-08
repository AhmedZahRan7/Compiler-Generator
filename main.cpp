#include <bits/stdc++.h>
#include "Lexical_Analyzer_Generator.hpp"
#include "Lexical_Analyzer.hpp"

using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now();
    auto lex_gen = new Lexical_Analyzer_Generator("test_rules.txt");
    auto lex = lex_gen->get_Lexical_Analyzer();
    lex->simulateProgramFile("test_program.txt");
    while (lex->getToken() != nullptr) {}
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time : " << duration.count()  << "ms" << '\n';

    return 0;
}
