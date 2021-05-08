#include "Lexical_Analyzer_Generator.hpp"

Lexical_Analyzer_Generator::Lexical_Analyzer_Generator(string rulesFilePath) {
    auto builder = new NFABuilder(rulesFilePath);
    auto nfa = builder->getNFA();
    auto dfa = new DFA(nfa);
    this->lex = new Lexical_Analyzer(dfa->getStates(), dfa->getTransitions(), dfa->getStartState());
}

Lexical_Analyzer* Lexical_Analyzer_Generator::get_Lexical_Analyzer() {
    if (lex != nullptr) return this->lex;
    return nullptr;
}
