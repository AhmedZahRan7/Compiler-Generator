#ifndef UTILITIES
#define UTILITIES
#include <bits/stdc++.h>
using namespace std;
const string CONCATIONATION_OPERATOR = "@";
const string POSTIVE_CLOSURE_OPERATOR = "+";
const string KLEENE_CLOSURE_OPERATOR = "*";
const string UNION_OPERATOR = "|";
const string RANGE_OPERATOR = "-";
string generateID();
string trim(string& str);
void removeSpaces(string &str);
vector<string> split(string& str);
vector<string> infixToPostfix(string expression);
bool isOperator(string c);
bool isHigherPrecedence(string c1, string c2);
vector<string> spiltInfixExpression(string expression);
bool isConcatination(string lastWord, string currentWord);
#endif