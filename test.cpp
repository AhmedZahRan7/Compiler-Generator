#include "utilities.hpp"
#include "CFGBuilder.hpp"
#include "Lexical_Analyzer_Generator.hpp"
#include "Parser_Generator/LLParserGenerator.hpp"

using namespace std::chrono;

int main() {
    auto lex_gen = new Lexical_Analyzer_Generator("Lexical_Analyzer_Generator\\lab_rules.txt");
    auto lex = lex_gen->get_Lexical_Analyzer();

    auto start = high_resolution_clock::now();
    CFGBuilder* cfg = new CFGBuilder("Parser_Generator\\test_rules_file.txt", *lex->getOrderedKeys());
    LLParserGenerator* generator =  new LLParserGenerator(cfg->getProcs());

    generator->printFirst();
    cout << endl;
    generator->printFollow();

    fstream file;
    file.open("lab_program.txt", ios::in);
    if (!file.is_open()) {
        cout << "File not exist";
        return 0;
    }
    auto parser = generator->generateParser(cfg->getTerminals());
    string line;
    cout << "\nStart Parsing:\n";
    while (getline(file,line)) {
        vector<string> words = split(line);
        for (auto w : words) {
            lex->simulate(w);
            Token* t;
            while ((t = lex->getNextToken()) != nullptr) {
                parser->parse(t);
            }
        }
    }
    file.close();
    Token* dollar_sign = new Token(new TokenKey("$"), new TokenValue("$"));
    parser->parse(dollar_sign);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << duration.count() << " us\n";
    return 0;
}
