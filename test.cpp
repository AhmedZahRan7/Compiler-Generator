#include "utilities.hpp"
#include "Terminal.hpp"
#include "NFABuilder.hpp"
#include "CFGBuilder.hpp"
#include "Lexical_Analyzer_Generator.hpp"
#include "Parser_Generator/LLParserGenerator.hpp"

// namespace std {
//     template<>
//     struct hash<Terminal> {
//         std::size_t operator()(const Elem& t) const {
//             using std::size_t;
//             using std::hash;
//             using std::string;

//             return hash<string>()(t.getId());
//         }
//     };
// };

int main() {
    auto lex_gen = new Lexical_Analyzer_Generator("Lexical_Analyzer_Generator\\lab_rules.txt");
    auto lex = lex_gen->get_Lexical_Analyzer();
    CFGBuilder* cfg = new CFGBuilder("Parser_Generator\\test_rules_file.txt", *lex->getOrderedKeys());
    LLParserGenerator* generator =  new LLParserGenerator(cfg->procList,cfg->allTerminals);
    generator->printFirst();
    generator->printFollow();
    return 0;
}
