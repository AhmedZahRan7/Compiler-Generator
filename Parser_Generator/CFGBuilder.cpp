#include "CFGBuilder.hpp"

CFGBuilder::CFGBuilder(string path, set<TokenKey*> lexTokenKeys) {
    this->rulesFile.open(path, ios::in);
    if (!this->rulesFile.is_open()) {
        cout << "File does not exist\n";
    } else {
        setTerminals(lexTokenKeys);
        buildCFG();
    }
}

void CFGBuilder::buildCFG() {
    string line;
    while (getline(this->rulesFile, line)) {
        removeSpaces(line);
        if (line.empty()) continue;
        parseRule(line);
    }
    this->rulesFile.close();
}

void CFGBuilder::parseRule(string& line) {
    unsigned int pos = 0;
    string lhs;
    if (line[pos] == '|') {
        if (!this->procList.empty()) {
            lhs = this->procList.back()->getLHS()->getId();
        }
        else {
            cout << "\nRule format is not correct for rule: " + line + '\n';
            return;
        }
    } else {
        lhs = parseLHS(line, pos);
        if (lhs.empty()) {
            cout << "\nlhsprintRule format is not correct for rule: " + line + '\n';
            return;
        }
    }
    pos++;

    vector<vector<Elem*>> rhs = parseRHS(line.substr(pos));
    if (rhs.empty()) {
        cout << "\nrhsprintRule format is not correct for rule: " + line + '\n';
        return;
    }
    auto it=rulesMapping.find(lhs);
    if(it == rulesMapping.end()){
        Production* proc;
        proc = new Production(addNonTerminal(lhs), rhs);
        this->procList.push_back(proc);
        rulesMapping[lhs]=proc;
    }
    else{
        Production* proc=rulesMapping[lhs];
        proc->addToRHS(rhs);
    }
}

// Parse left hand side which is a non empty string before the assignment operator.
string CFGBuilder::parseLHS(string& line, unsigned int& pos) {
    string lhs;
    bool assignmentExist = false;
    while (pos < line.size()) {
        if (line[pos] == '=') {
            assignmentExist = true;
            break;
        }
        lhs += line[pos];
        pos++;
    }
    if (!assignmentExist || pos == 0) return "";
    trim(lhs);
    return lhs;
}

// Parse right hand side which is a non empty combination of terminals and non terminals.
vector<vector<Elem*>> CFGBuilder::parseRHS(string line) {
    trim(line);
    vector<vector<Elem*>> rhs;
    string buffer;
    bool terminalFlag = false;
    rhs.push_back(vector<Elem*>());
    unsigned int currVec = 0;
    for (char c : line) {
        if (c == '\'') {
            if (terminalFlag) {
                terminalFlag = false;
                 if (buffer=="epsilon" || buffer =="EPSILON" || buffer =="\\L"){
                    rhs[currVec].push_back((Elem*)EPSILON);
                 }
                 else{
                    rhs[currVec].push_back(checkTerminal(buffer));
                 }
            } else {
                if (!buffer.empty()) rhs[currVec].push_back(addNonTerminal(buffer));
                terminalFlag = true;
            }
            buffer = "";
        } else if (c == '|') {
            // if a non terminal comes before it
            if (!buffer.empty()) rhs[currVec].push_back(addNonTerminal(buffer));
            rhs.push_back(vector<Elem*>());
            currVec++;
            buffer = "";
        } else if (c == ' ') {
            if (!buffer.empty()) rhs[currVec].push_back(addNonTerminal(buffer));
            buffer = "";
        } else {
            buffer += c;
        }
    }
    if (!buffer.empty()) rhs[currVec].push_back(addNonTerminal(buffer));

    // There is a closing quote missing so return an empty vector to be interpreted as error in format.
    if (terminalFlag) rhs.clear();
    return rhs;
}

NonTerminal* CFGBuilder::addNonTerminal(string buffer) {
    trim(buffer);
    auto it = this->nonTerminalsMapping.find(buffer);
    if (it == this->nonTerminalsMapping.end()) {
        NonTerminal* nt = new NonTerminal(buffer);
        this->nonTerminalsMapping[buffer] = nt;
    }
    return this->nonTerminalsMapping[buffer];
}

Terminal* CFGBuilder::checkTerminal(string buffer) {
    trim(buffer);
    auto it = this->terminalsMapping.find(buffer);
    if (it == this->terminalsMapping.end()) {
        cout << "Terminal " + buffer +  " is not found in lexical analyzer tokens.\n";
        cout << "Add it to the parser terminals.\n";
        Terminal *t = new Terminal(buffer);
        this->terminalsMapping[buffer] = t;
    }
    return this->terminalsMapping[buffer];
}

void CFGBuilder::setTerminals(set<TokenKey*> lexTokenKeys) {
    for (auto tk : lexTokenKeys) {
        this->terminalsMapping[tk->getKey()] = new Terminal(tk->getKey());
    }
}

unordered_map<string, Terminal*> CFGBuilder::getTerminals() {
    return this->terminalsMapping;
}

vector<Production*> CFGBuilder::getProcs() {return this->procList;}

void CFGBuilder::printProductions() {
    for (auto p : this->procList) cout << p->toString();
    cout << endl;
}
