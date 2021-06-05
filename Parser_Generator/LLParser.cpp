#include "LLParser.hpp"

LLParser::LLParser(NonTerminal* startState, unordered_map<string, Terminal*> terminalsMapping) {
    this->terminalsMapping = terminalsMapping;
    this->LLStack.push(DOLLAR_SIGN);
    this->LLStack.push(startState);
}


bool match(Terminal* a, Terminal* b) { return a == b; }

void LLParser::addToTable(NonTerminal* lhs, Terminal* symbol, vector<Elem*> elements) {
    vector<Elem*> check = this->parsingTable[lhs][symbol];
    if (!check.empty()) {
        cout << "\nThe grammar is ambiguous as there are two production rules in the same entry\n";
        cout << lhs->getId() << " -" << symbol->getId() << "-> ";
        for (auto e : check) cout << e->getId();
        cout << endl;
        cout << lhs->getId() << " -" << symbol->getId() << "-> ";
        for (auto e : elements) cout << e->getId() << " ";
        cout << endl;
        exit(1);
    }
    this->parsingTable[lhs][symbol] = elements;
}

bool LLParser::existInTable(NonTerminal* lhs, Terminal* symbol) {
    auto row_it = this->parsingTable.find(lhs);
    if (row_it != this->parsingTable.end()) {
        return this->parsingTable[lhs].find(symbol) != this->parsingTable[lhs].end();
    }
    return false;
}

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
    // cout << lhs->getId() << " ---> ";
    // for (auto t : this->parsingTable[lhs][input]) {
    //     cout << t->getId() << " ";
    // }
    stack<Elem*> tmpStack;
    while (!this->LLStack.empty()) {
        tmpStack.push(this->LLStack.top());
        this->LLStack.pop();
    }

    while (!tmpStack.empty()) {
        cout << tmpStack.top()->getId() << " ";
        this->LLStack.push(tmpStack.top());
        tmpStack.pop();
    }
    cout << '\n';
}
