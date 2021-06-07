#include "CFG.hpp"

CFG::CFG(vector<Production*> rules) {
    this->rules = rules;
    eliminateLeftRecursion();
    eliminateLeftRefactoring();
}

// putIn Aa | b in place of S
// rhs = Ac | dSe | f, vecPos = 1, NonTPos = 1
// rhs = Ac | f | dAae | dbe
void insertIntoRHS(unsigned int NonTPos, unsigned int vecPos, vector<vector<Elem*>> putIn, vector<vector<Elem*>>& rhs) {
    // Store the target vector in repeatedVec and erase the non terminal from it before inserting its replacement.
    // Also remove the target vector from the rhs before inserting the new ones.
    vector<Elem*> repeatedVec = rhs[vecPos];
    repeatedVec.erase(repeatedVec.begin() + NonTPos);
    for (vector<Elem*> v : putIn) {
        vector<Elem*> currV = repeatedVec;
        currV.insert(currV.begin() + NonTPos, v.begin(), v.end());
        rhs.push_back(currV);
    }
}

bool match(NonTerminal* a, NonTerminal* b) { return a == b; }

void removeIndicesFromVector(std::vector<vector<Elem*>>& v, std::vector<int> rm ) {
    std::for_each(rm.crbegin(), rm.crend(), [&v](auto index) { v.erase(begin(v) + index); });
}

// S --> Aa | b
// A --> Ac | dSe | f
// then A --> Ac | f | dAae | dbe
void replaceProduction(Production* putIn, Production* proc) {
    NonTerminal* nonTtoPutIn = putIn->getLHS();
    vector<vector<Elem*>> rhs = proc->getRHS();
    vector<int> replacedVecIndicesToBeDeleted;
    // Stored right hand side in len to stop the replace after consuming the original rhs.
    unsigned int len = rhs.size();
    for (unsigned int i = 0; i < len; ++i) {
        for (unsigned int j = 0; j < rhs[i].size(); ++j) {
            if (isNonTerminal(rhs[i][j]) && match((NonTerminal*) rhs[i][j], nonTtoPutIn)) {
                insertIntoRHS(j, i, putIn->getRHS(), rhs);
                replacedVecIndicesToBeDeleted.push_back(i);
                break;
            }
        }
    }
    removeIndicesFromVector(rhs, replacedVecIndicesToBeDeleted);
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

void CFG::eliminateLeftRefactoring(){
    pair<unsigned int,unsigned int> pair ={1,0};
    for (unsigned int i = 0; i < rules.size(); ++i) {
    vector<vector<Elem*>> rhs = rules[i]->getRHS();
    vector<int> commonElemContainer=commonElemIndeces(rhs); //vector for indexes for which components has the same commonElem
     if(commonElemContainer.size()!=0){
    if(pair.second != i){
        pair.first=1;
    }
     int CountOFCommonElements =countCommon(commonElemContainer, rhs);
     vector<Elem*> commonElems;
     for(unsigned int ii=0;ii<CountOFCommonElements;ii++){
     commonElems.push_back(rhs[commonElemContainer[0]][ii]);
     }
     vector<vector<Elem*>> newRhs;
      for(unsigned int ii=0;ii<commonElemContainer.size();ii++){
          vector<Elem*> myvec=rhs[commonElemContainer[ii]];
          myvec.erase(myvec.begin(), myvec.begin() + CountOFCommonElements);
          if(myvec.empty()){
              myvec.push_back(EPSILON);
          }
          newRhs.push_back(myvec);
      }
     removeIndicesFromVector(rhs, commonElemContainer);
     NonTerminal* newLHS = new NonTerminal(rules[i]->getLHS()->getId() + string(pair.first++,factoringMark));
     pair.second=i;
     commonElems.push_back(newLHS);
     rhs.push_back(commonElems);
     rules[i]->setRHS(rhs);
     Production* newProc = new Production(newLHS,newRhs);
     rules.push_back(newProc);
     i--;
    }

    }
}
vector<int> CFG:: commonElemIndeces(vector<vector<Elem*>> rhs){
   Elem* commonElem;
   vector<int> commonElemContainer;
   for (unsigned int j=0;j<rhs.size();j++) {
        commonElem=rhs[j][0];
        commonElemContainer.push_back(j);
            for (unsigned int k=j+1;k<rhs.size();k++) {
                if(commonElem==rhs[k][0]){
                  commonElemContainer.push_back(k);
                }
            }
            if(commonElemContainer.size()>1){
                break;
            }
            else{
            commonElemContainer.clear();
            }
        }
    return commonElemContainer;
}
int CFG:: countCommon(std::vector<int> commonElemContainer,vector<vector<Elem*>> rhs){
unsigned int CountOFCommonElements=1;
         bool flag=true;
         while(flag){
            for(unsigned int ii=1;ii<commonElemContainer.size();ii++){
                if(CountOFCommonElements<rhs[commonElemContainer[ii]].size() && CountOFCommonElements<rhs[commonElemContainer[0]].size()){
                    if(rhs[commonElemContainer[0]][CountOFCommonElements]!=rhs[commonElemContainer[ii]][CountOFCommonElements]){
                   flag=false;
                  }
                }
                else {
                    flag=false;
                }
            }

            if(flag){
                CountOFCommonElements++;
            }
         }
  return CountOFCommonElements;
}

vector<Production*> CFG::getProcs() { return this->rules; }
