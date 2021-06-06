#include "CFG.hpp"

CFG::CFG(vector<Production*> rules) {
    this->rules = rules;
    eliminateLeftRecursion();
    // eliminateLeftRefactoring();
}

// putIn Aa | b in place of S
// rhs = Ac | dSe | f, vecPos = 1, NonTPos = 1
// rhs = Ac | f | dAae | dbe
void insertIntoRHS(unsigned int NonTPos, unsigned int vecPos, vector<vector<Elem*>> putIn, vector<vector<Elem*>>& rhs) {
    // Store the target vector in repeatedVec and erase the non terminal from it before inserting its replacement.
    // Also remove the target vector from the rhs before inserting the new ones.
    vector<Elem*> repeatedVec = rhs[vecPos];
    repeatedVec.erase(repeatedVec.begin() + NonTPos);
    rhs.erase(rhs.begin() + vecPos);
    for (vector<Elem*> v : putIn) {
        vector<Elem*> currV = repeatedVec;
        currV.insert(currV.begin() + NonTPos, v.begin(), v.end());
        rhs.push_back(currV);
    }
}

bool match(NonTerminal* a, NonTerminal* b) { return a == b; }

// S --> Aa | b
// A --> Ac | dSe | f
// then A --> Ac | dAae | dbe | f
void replaceProduction(Production* putIn, Production* proc) {
    NonTerminal* nonTtoPutIn = putIn->getLHS();
    vector<vector<Elem*>> rhs = proc->getRHS();
    for (unsigned int i = 0; i < rhs.size(); ++i) {
        for (unsigned int j = 0; j < rhs[i].size(); ++j) {
            if (isNonTerminal(rhs[i][j]) && match((NonTerminal*) rhs[i][j], nonTtoPutIn)) {
                insertIntoRHS(j, i, putIn->getRHS(), rhs);
            }
        }
    }
    proc->setRHS(rhs);
}

bool leftRecursionExist(Production* proc) {
    for (vector<Elem*> v : proc->getRHS()) {
        if (isNonTerminal(v.front()) && match((NonTerminal*) v.front(), proc->getLHS())) {
            return true;
        }
    }
    return false;
}

// A --> Ac | Aad | bd | f
// A --> bdA# | fA#
// A# --> cA# | adA# | Epsilon
Production* eliminateImmediateLeftRecursion(Production* proc) {
    NonTerminal* newLHS = new NonTerminal(proc->getLHS()->getId() + newNonTMark);
    vector<vector<Elem*>> startWithLHS;

    vector<vector<Elem*>> rhs = proc->getRHS();
    for (auto vecIt = rhs.begin(); vecIt != rhs.end();) {
        if (isNonTerminal(vecIt->front()) && match((NonTerminal*) vecIt->front(), proc->getLHS())) {
            vecIt->erase(vecIt->begin());
            vecIt->push_back(newLHS);
            startWithLHS.push_back(*vecIt);
            rhs.erase(vecIt);
        } else {
            vecIt->push_back(newLHS);
            ++vecIt;    // Only increment when not erasing from vector.
        }
    }
    if (rhs.empty()) {
        cout << "Left recursion can not be eliminated from this production.\n" << proc->toString();
        exit(1);
    }
    proc->setRHS(rhs);
    startWithLHS.push_back({EPSILON});
    return new Production(newLHS, startWithLHS);
}

void CFG::eliminateLeftRecursion() {
    vector<Production*> resultOfImmediateRecursion;
    for (unsigned int i = 0; i < rules.size(); ++i) {
        for (unsigned int j = 0; j < i; ++j) {
            replaceProduction(rules[j], rules[i]);
        }
        if (leftRecursionExist(rules[i])) {
            Production* newProc = eliminateImmediateLeftRecursion(rules[i]);
            resultOfImmediateRecursion.push_back(newProc);
        }
    }
    this->rules.insert(this->rules.end(), resultOfImmediateRecursion.begin(), resultOfImmediateRecursion.end());
}

vector<Production*> CFG::getProcs() { return this->rules; }
