#include "CFG.hpp"

CFG::CFG(vector<Production*> rules) {
    this->rules = rules;
    //eliminateLeftRecursion();
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

void CFG::eliminateLeftRefactoring(){
    cout<<"welcome"<<endl;
    pair<int,int> pair ={1,0};
    for (unsigned int i = 0; i < rules.size(); ++i) {
    vector<vector<Elem*>> rhs = rules[i]->getRHS();
    vector<int> commonElemContainer=commonElemIndeces(rhs); //vector for indexes for which components has the same commonElem
     cout<<"container"<<endl;
     if(commonElemContainer.size()!=0){
    if(pair.second != i){
        pair.first=1;
    }
     int CountOFCommonElements =countCommon(commonElemContainer, rhs);
     cout<<"count ->"<<CountOFCommonElements<<endl;
     vector<Elem*> commonElems;
     for(int ii=0;ii<CountOFCommonElements;ii++){
     commonElems.push_back(rhs[commonElemContainer[0]][ii]);
     }
     cout<<"wel"<<endl;
     vector<vector<Elem*>> newRhs;
      for(int ii=0;ii<commonElemContainer.size();ii++){
          vector<Elem*> myvec=rhs[commonElemContainer[ii]];
          myvec.erase(myvec.begin(), myvec.begin() + CountOFCommonElements);
          if(myvec.empty()){
              myvec.push_back(EPSILON);
          }
          newRhs.push_back(myvec);
      }
      cout<<"before"<<endl;
     removeIndicesFromVector(rhs, commonElemContainer);
     cout<<"after"<<endl;
     NonTerminal* newLHS = new NonTerminal(rules[i]->getLHS()->getId() + string(pair.first++,factoringMark));
     pair.second=i;
     cout<<"after1"<<endl;
     commonElems.push_back(newLHS);
     cout<<"after2"<<endl;
     rhs.push_back(commonElems);
     rules[i]->setRHS(rhs);
     cout<<"after3"<<endl;
     Production* newProc = new Production(newLHS,newRhs);
     cout<<"after4"<<endl;
     rules.push_back(newProc);
     i--;
    }
    
    }
cout<<"end"<<endl;
}
vector<int> CFG:: commonElemIndeces(vector<vector<Elem*>> rhs){
   Elem* commonElem;
   vector<int> commonElemContainer;
   for (int j=0;j<rhs.size();j++) {
        commonElem=rhs[j][0];
        commonElemContainer.push_back(j);
            for (int k=j+1;k<rhs.size();k++) {
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
    cout<<"////////////////////////"<<rhs.size()<<endl;
int CountOFCommonElements=1;
for(int ii=0;ii<commonElemContainer.size();ii++){
    cout<<commonElemContainer[ii]<<"//";
}
         bool flag=true;
         while(flag){
            for(int ii=1;ii<commonElemContainer.size();ii++){
                cout<<"here11111";
                if(CountOFCommonElements<rhs[commonElemContainer[ii]].size() && CountOFCommonElements<rhs[commonElemContainer[0]].size()){
                    if(rhs[commonElemContainer[0]][CountOFCommonElements]!=rhs[commonElemContainer[ii]][CountOFCommonElements]){
                   cout<<"here";
                   flag=false;
                  }
                }
                else {
                    flag=false;
                }
            }

            if(flag){
                cout<<"CountOFCommonElements"<<endl;
                CountOFCommonElements++;
            }
         }
         cout<<"////////////////////////"<<endl;
  return CountOFCommonElements;
}
void CFG::removeIndicesFromVector(std::vector<vector<Elem*>>& v, std::vector<int> rm )
{
    std::for_each(rm.crbegin(), rm.crend(), [&v](auto index) { v.erase(begin(v) + index); });
}

vector<Production*> CFG::getProcs() { return this->rules; }
