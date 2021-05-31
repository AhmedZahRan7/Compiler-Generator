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
vector<string> split(string& str){
    stringstream stream(str);
    string word;
    vector<string> vec;
    while (stream >> word) vec.push_back(word);
    return vec;
}
/**
 * @brief convert regular definations and regular expressions
 * into the postfix representation using Shunting-yard algorithm
 * the operators of regular definations and regular expressions inorder are
 * )
 * -
 * + *
 * concatination
 * |
 * (
 *
 * l(l|d)* --> l l d | *
 * @param expression infix representation of regular definations or regular expressions
 * @return string postfix representation of regular definations or regular expressions
 */
vector<string> infixToPostfix(string expression){
    vector<string> postFix;
    stack<string> operators;
    vector<string> splittedExpression = spiltInfixExpression(expression);
    unsigned int i=0;
    unsigned int lastI=0;
    for(i=0;i<splittedExpression.size();i++){
        string word = splittedExpression[i];
        if(i>0 && i>lastI &&isConcatenation(splittedExpression[i-1],splittedExpression[i])){
            word = CONCATENATION_OPERATOR;
            lastI = i--;
        }
        if(isOperator(word)){
            if(word=="(") operators.push(word);
            else if(word==")") {
                while(operators.top() != "("){
                    postFix.push_back(operators.top());
                    operators.pop();
                }
                operators.pop();
            }
            else {
                while(!operators.empty()
                && isHigherPrecedence(operators.top(),word)){
                    postFix.push_back(operators.top());
                    operators.pop();
                }
                operators.push(word);
            }
        }
        else{
            postFix.push_back(word);
        }
    }
    while(!operators.empty()){
        postFix.push_back(operators.top());
        operators.pop();
    }
    return postFix;
}
/**
 * @brief This function handels the problem of the absence of the concatination operator
 * as each regex operation is represented with an operator except the concationation.
 *
 * @param lastWord the last word inserted into the postfix expression
 * @param currentWord the word to be inserted into the postfix expression
 * @return true if we need to consider concatination operator between these two words
 * @return false if not
 */
bool isConcatenation(string lastWord, string currentWord){
    return lastWord != "(" && lastWord != UNION_OPERATOR && lastWord != RANGE_OPERATOR &&
    (currentWord == "(" || !isOperator(currentWord));
}
vector<string> spiltInfixExpression(string expression){
    string expressionWithSpaces = "";
    for(unsigned int i=0 ;i<expression.size();i++){
        string s = "";
        s += expression[i];
        if(isOperator(s)&&(i==0 || expression[i-1]!='\\')) expressionWithSpaces += ' ';
        expressionWithSpaces += s;
        if(isOperator(s)&&(i==0 || expression[i-1]!='\\')) expressionWithSpaces += ' ';
    }
    removeSpaces(expressionWithSpaces);
    return split(expressionWithSpaces);
}
bool isOperator(string c){
    return
    c ==  RANGE_OPERATOR ||
    c == UNION_OPERATOR ||
    c == POSITIVE_CLOSURE_OPERATOR ||
    c == KLEENE_CLOSURE_OPERATOR ||
    c == "(" ||
    c == ")" ||
    c == CONCATENATION_OPERATOR
    ;
}
unordered_map<string,int> charPrecedence {
    {UNION_OPERATOR,0},
    {CONCATENATION_OPERATOR,1},
    {POSITIVE_CLOSURE_OPERATOR,2},
    {KLEENE_CLOSURE_OPERATOR,2},
    {RANGE_OPERATOR,3},
};
bool isHigherPrecedence(string c1, string c2){
    return charPrecedence[c1] > charPrecedence[c2];
}
