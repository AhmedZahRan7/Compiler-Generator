#include "LLParser.hpp"

LLParser::LLParser(NonTerminal* startState, unordered_map<NonTerminal*, unordered_map<Terminal*, vector<Elem*> >> parsingTable,
                   unordered_map<string, Terminal*> terminalsMapping) {
    this->parsingTable = parsingTable;
    this->terminalsMapping = terminalsMapping;
    this->LLStack.push(DOLLAR_SIGN);
    this->LLStack.push(startState);
}

bool isTerminal(Elem* e) { return dynamic_cast<Terminal*>(e) != nullptr; }

bool isNonTerminal(Elem* e) { return dynamic_cast<NonTerminal*>(e) != nullptr; }

bool isSync(Elem* e) { return e == SYNC; }

bool isEpsilon(Elem* e){ return e == EPSILON; }

bool match(Terminal* a, Terminal* b) { return a == b; }

void LLParser::parse(Token* input) {
    Elem* top = this->LLStack.top();
    string tk = input->getKey()->getKey();
    Terminal* currT = terminalsMapping[tk];
    while (isNonTerminal(top)) {
        vector<Elem*> tableResult = parsingTable[(NonTerminal*) top][currT];
        if (tableResult.empty()) { // Case 1: discard this terminal and break if it goes to nothing in the table.
            break;
        }
        LLStack.pop();

        if (!isSync(tableResult.front())) { // Case 2: if it's sync then pop top of stack only --> panic mode error recovery.
            output((NonTerminal*) top, currT);
            if (!isEpsilon(tableResult.front())) { // Case 3: if the result from table is epsilon.
                for (auto it = tableResult.rbegin(); it != tableResult.rend(); ++it) {
                    this->LLStack.push(*it);
                }
            }
        }

        top = this->LLStack.top();
    }

    if (isTerminal(top)) {
        if (match((Terminal*) top, currT)) {
            LLStack.pop();
            if (tk == "$" && LLStack.empty()) {
                cout << "accept" << endl;
            }
        }
        else {
            LLStack.pop();  // Error: Recovery --> missing terminal in stack is inserted.
            parse(input);
        }
    }
}

void LLParser::output(NonTerminal* lhs, Terminal* input) {
    cout << lhs->getId() << " ---> ";
    for (auto t : this->parsingTable[lhs][input]) {
        cout << t->getId();
    }
    cout << '\n';
}
