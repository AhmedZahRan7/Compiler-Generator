#ifndef Production_HPP
#define Production_HPP
#include "NonTerminal.hpp"
#include "Terminal.hpp"
#include "Elem.hpp"
#include "utilities.hpp"

/**
 * @brief The Production specify how the terminals and non terminals are combined
 * together to form strings.
 * Ex: A --> aBc | d | eps
 * where {A, B} are non terminals and {a, c, d, eps} are terminals.
 * A is the left hand side, and aBc | d | eps is the right hand side.
 */
class Production {
private:
    NonTerminal* lhs;
    vector<vector<Elem*>> rhs;
public:
    Production();
    Production(NonTerminal* lhs, vector<vector<Elem*>>& rhs);
    NonTerminal* getLHS();
    vector<vector<Elem*>> getRHS();
    void addToRHS(vector<vector<Elem*>> anotherRHS);
};

#endif
