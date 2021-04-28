#include "utilities.hpp"
string generateID(){
    static string ID = "A";
    string returnString = ID;
    int lastIdx = ID.size()-1;
    if(ID[lastIdx] == 'Z'){
        while(lastIdx>=0){
            if(ID[lastIdx] == 'Z') ID[lastIdx] = 'A';
            else{
                ID[lastIdx]++;
                break;
            }
            lastIdx--;
        }
        if(lastIdx<0) ID.push_back('A');
    }
    else ID[lastIdx]++;
    return returnString;
}