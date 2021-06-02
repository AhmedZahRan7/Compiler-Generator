#include "CFGBuilder.hpp"

CFGBuilder::CFGBuilder(string path, set<TokenKey*> lexTokenKeys) {
    this->rulesFile.open(path, ios::in);
    if (!this->rulesFile.is_open()) {
        cout << "File does not exist\n";
    } else {
        setTerminals(lexTokenKeys);
        buildCFG();
        printProductions();
    }
}

void CFGBuilder::buildCFG() {
    string line, nextLine;
    while (getline(this->rulesFile, line)) {
        while (getline(this->rulesFile, nextLine)) {
            removeSpaces(nextLine);
            if (nextLine[0] == '|') {
                line += ' ' + nextLine;
                nextLine = "";
            } else {
                break;
            }
        }
        parseRule(line);
        if (!nextLine.empty()) parseRule(nextLine);

        if (this->rulesFile.eof()) break;
    }

    this->rulesFile.close();
}

void CFGBuilder::parseRule(string& line) {
    if (line.size() == 0) return;
    unsigned int pos = 0;
    string lhs = parseLHS(line, pos);
    if (lhs.empty()) {
        cout << "\nRule format is not correct for rule: " + line + '\n';
        return;
    }

    pos++;
    vector<vector<Elem*>> rhs = parseRHS(line.substr(pos));
    if (rhs.empty()) {
        cout << "\nRule format is not correct for rule: " + line + '\n';
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
                // if (buffer=="epsilon" || buffer =="EPSILON" || buffer =="\\L"){
                //    rhs[currVec].push_back((Elem*)EPSILON);
                // }
                // else{
                //    rhs[currVec].push_back(checkTerminal(buffer));
                // }
                rhs[currVec].push_back(checkTerminal(buffer));
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
    // NonTerminal* t = new NonTerminal(trim(buffer));
    // auto it = this->allNonTerminals.find(t);
    // if (it != this->allNonTerminals.end()) {
    //     this->allNonTerminals.insert(t);
    //     return t;
    // }
    // delete t;
    // return *it;
    trim(buffer);
    for (auto t : this->allNonTerminals) {
        if (buffer == t->getId()) return t;
    }
    NonTerminal* t = new NonTerminal(buffer);
    this->allNonTerminals.insert(t);
    return t;
}

Terminal* CFGBuilder::checkTerminal(string buffer) {
    // Terminal* t = new Terminal(trim(buffer));
    // auto it = this->allTerminals.find(t);
    // if (it != this->allTerminals.end()) {
    //     this->allTerminals.insert(t);
    //     return t;
    // }
    // delete t;
    // return *it;
    trim(buffer);
    for (auto t : this->allTerminals) {
        if (buffer == t->getId()) return t;
    }
    // Todo: exception handling
    Terminal* t = new Terminal(buffer);
    this->allTerminals.insert(t);
    return t;
}

void CFGBuilder::setTerminals(set<TokenKey*> lexTokenKeys) {
    for (auto tk : lexTokenKeys) {
        this->allTerminals.insert(new Terminal(tk->getKey()));
    }
}

void CFGBuilder::printProductions() {
    for (auto p : this->procList) {
        cout << endl << p->getLHS()->getId() << " --> ";
        vector<vector<Elem*>> rhs = p->getRHS();
        for (unsigned int i = 0; i < rhs.size(); ++i) {
            for (auto e : rhs[i]) {
                cout << e->getId() << " ";
            }
            if (i != rhs.size() - 1) cout << "|| ";
        }
        cout << endl;
    }
    cout << endl;
}
