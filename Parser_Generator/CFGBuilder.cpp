#include "CFGBuilder.hpp"

CFGBuilder::CFGBuilder(string path) {
    this->rulesFile.open(path, ios::in);
    if (!this->rulesFile.is_open()) {
        cout << "File does not exist\n";
    } else {
        buildCFG();
    }
}

void CFGBuilder::buildCFG() {}
