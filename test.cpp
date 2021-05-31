#include "utilities.hpp"
#include "Terminal.hpp"
#include "NFABuilder.hpp"
#include "CFGBuilder.hpp"
#include "Lexical_Analyzer_Generator.hpp"

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
    new CFGBuilder("Parser_Generator\\parser_rules_file.txt", *lex->getOrderedKeys());
    return 0;
}
