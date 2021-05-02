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

string trim(string& str){
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
    return str;
}

void removeSpaces(string &str){
    string str2;
    for(char c:str){
        if(c==' ' && str2.back()==' ') continue;
        str2 += c; 
    }
    str = trim(str2);
}