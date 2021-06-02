#include "utilities.hpp"
#include "Terminal.hpp"
#include "NFABuilder.hpp"
#include "CFGBuilder.hpp"
#include "Lexical_Analyzer_Generator.hpp"
#include "Parser_Generator/LLParserGenerator.hpp"

using namespace std::chrono;

int main() {
    auto lex_gen = new Lexical_Analyzer_Generator("Lexical_Analyzer_Generator\\lab_rules.txt");
    auto lex = lex_gen->get_Lexical_Analyzer();
    auto start = high_resolution_clock::now();
    CFGBuilder* cfg = new CFGBuilder("Parser_Generator\\test_rules_file.txt", *lex->getOrderedKeys());
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << duration.count() << " us\n";
    // LLParserGenerator* generator =  new LLParserGenerator(cfg->getProcs(), cfg->getTerminals());
    // generator->printFirst();
    // generator->printFollow();
    return 0;
}
