# Compiler Generator
In this repo the two front-end compiler [Lexiacal-Analyzer Generator and Parser Generator] implemented in C++
contributors:
>Ahmed Ali
Abobakr Abdelaziz
Elsayed Akram
Fares Medhat
#### The Lexiacal-Analyzer Generator
##### Overview
- What is lexical analysis?  lexical analysis or tokenization is the process of converting a sequence of characters (lexemes)- such as in a computer program - into a sequence of tokens - strings with an assigned and thus identified meaning -
- What is the lexical analyzer generator? A lexical analyzer generator is a program designed to generate lexical analyzers, which recognize lexical patterns in text. The lexical analyzer generator is required to automatically construct a lexical analyzer from a regular expression description (rules) of a set of tokens. 
##### Algorithms
- Shunting-yard algorithm:
Shunting-yard algorithm converts regular definitions and regular expressions into the postfix representation to be easily calculated for constructing the NFA.
`Ex : l ( l | d ) * --> l l d | *`
- NFA Builder:
The NFA builder implements a stack<NFA*> to keep track of the latest build NFA to perform Thompson construction algorithm on it.
For the previous example stack is `[ l ] -> [ l , l ] -> [ l , l , d ] -> [l , (l|d) ] -> [l , (l|d)* ] ->[ l(l|d)* ]`
- Subset Construction:
The algorithm constructs a transition table for the DFA. Each state of the DFA is a set of NFA states. The algorithm needs functions to work properly on the NFA states : 
1. epsClosure(s): gets all the states that move with EPS transition from current state by putting the input states in a stack then getting all states that go with EPS transition and push them into the stack until all states with EPS transition are reached.
1. epsClosure(T): works the same as epsClosure(s) but on a set of states T.
1. move(s, condition): the function gets the set of states that s moves to on a specific condition from the transition table.
- Minimization of DFA
1. Maps each set of states by its accepting token into a `unorderd_map<TokenKey, set<State*>>` and the non accepting ones are mapped into a temporary token “NonAccepting”.
1. Then looping over all input symbols and use symbol x to try to split each group into smaller subgroups.
1. In `partition(groups, x)` function, we loop over groups, and each group we loop over a states s and mark this state (this mark means it’s been put in a subgroup), then loop over other the rest of states and try the input x on each of them, if a state v goes to the same group then it’s put in the same subgroup with state s and also marked.

##### Example :

For the following rules and test program 
```
letter = a-z | A-Z
digit = 0 - 9
{boolean int float}
{while if else}
id: letter (letter | digit)*
digits = digit+
num: digit+ | digit+ . digits (\L | E digits)
relop: \=\= | !\= | > | >\= | < | <\=
assign: \=
[; , \( \) { }]
addop: \+ | \-
mullop: \* | /
```
```
int sum , count , pass , mnt; while (pass != 10 ){
pass = pass + 1;
}
boolean float int intx
```
The output tokens are 
`{int, int} {id, sum} {id, count} {id, pass} {id, mnt} {while, while} {id, pass} {relop, !=} {num, 10} {id, pass} {assign, =} {id, pass} {addop, +} {num, 1} {boolean, boolean} {float, float} {int, int} {id, intx}`


#### The Parser Generator
##### Overview
- A top-down parser that uses a one-token lookahead is called an LL(1) parser.
The first L indicates that the input is read from left to right.
The second L says that it produces a left-to-right derivation.
And the 1 says that it uses one lookahead token.
The parser needs to find a production to use for nonterminal N when it sees lookahead token t.
To select which production to use, it suffices to have a table that has, as a key, a pair (N, t) and gives the number of a production to use.

##### Algorithms
- Algorithms for parsing the input file:
Read each line and parse it to get the left hand side and right hand side of a production.
```
if the line starts with ‘|’: Parse the right hand side and add it to the last production
Parse the left hand side before the ‘=’ sign
Parse the right hand side after the ‘=’ sign and at each ‘|’ create new vector to push it to the right hand side vector of vectors
```
- Eliminate left-recursion:
A grammar cannot be immediately left-recursive, but it still can be left-recursive. By just eliminating the immediate left-recursion, we may not get a grammar which is not left-recursive.
```
for i from 1 to n do {
   for j from 1 to i-1 do {
      replace each production
      Ai -> Aj y by Ai -> a1 y | .... | ak y 
      where Aj -> a1 | ... | ak
   }
   eliminate immediate left-recursions among Ai productions
}
```

- Eliminate left-refactoring:
A predictive parser (a top-down parser without backtracking) insists that the grammar must be left-factored.
Grammar ->  a new equivalent grammar suitable for predictive parsing.
`
For example :
let grammar is : A -> xB | xC | yD | yE | F
Should turned into
A -> xG | yH | F 
G -> B | C
H -> C | D 
`
```
For i from 0 to rules.size() {
  Get indices for vectors with the same first Element
  Get the number of longest common prefix between them
  Push this common elements to vector and new non lhs will 
  Pushed back to this vector
  This vector will be pushed to rhs
  New rhs will have the vectors of elements without common prefix
  Remove the vectors with the same prefix from rhs
  set rhs now
  Make new production with new non terminal and new rhs
  This new production will pushed to vector of rules
}
```
- Generate First Set:
FIRST(X) for a grammar symbol X is the set of terminals that begin the strings derivable from X.
Rules to construct the first set :
```
If X is a terminal symbol FIRST(X)={X}
If X is a non-terminal symbol and X -> E is a production rule then E is in FIRST(X)
If X is a non-terminal symbol and X -> Y1 Y2..Yn is a production rule and if a terminal t in FIRST(Yi) and E is in all FIRST(Yj) for j=1,...,i-1 then t is in FIRST(X).
if E is in all FIRST(Yj) for j=1,...,n then E is in FIRST(X).
```
- Generate  Follow Set:
FOLLOW(X) for a grammar symbol X is the set of the terminals which occur immediately after (follow) the non-terminal X in the strings derived from the starting symbol.
Rules to construct the follow set :
```
If $ is the start symbol $ is in FOLLOW(S)
If A -> xBy is a production rule >> everything in FIRST(y) is FOLLOW(B) except E
If ( A -> xB is a production rule ) or ( A -> xBy is a production rule and E is in FIRST(y)) >> everything in FOLLOW(A) is in FOLLOW(B).
```
- Generate parsing table from follow and first :
What is the Parsing Table?
A two-dimensional array M[A,a]
Each row is a non-terminal symbol
Each column is a terminal symbol or the special symbol $
Each entry holds a production rule.

    Rules to build the parsing table :
```
for each production rule A -> x of a grammar G :
   for each terminal a in FIRST(x) add A -> x to M[A,a]
   If E in FIRST(x) for each terminal a in FOLLOW(A) add A -> x to M[A,a]
   If E in FIRST(x) and $ in FOLLOW(A)  add A -> x to M[A,$]
   If the First(x) not contains E then for each terminal t in follow(A) : if M[A,t] = SYNC if not empty
```


- Parsing Actions:
The symbol at the top of the stack (say X) and the current symbol in the input string (say a) determine the parser action.
There are four possible parser actions.
```
1. If X and a are $  parser halts (successful completion)

2. If X and a are the same terminal symbol (different from $) parser pops X from the stack, and moves the next symbol in the input buffer.

3. If X is a non-terminal parser look at the parsing table entry M[X,a]. If M[X,a] holds a production rule XY1Y2...Yk, it pops X from the stack and pushes Yk,Yk-1,...,Y1 into the stack. The parser also outputs the production rule XY1Y2 ...Yk to represent a step of the derivation.

4. If none of the above then error:
    all empty entries in the parsing table are errors.
    If X is a terminal symbol different from a, this is also an error case.
```

##### Data Structures :

```
Class Elem :
   string id

Class NonTerminal : Elem
Class Terminal : Elem

Class Production :
   NonTerminal* lhs
   vector<vector<Elem*>> rhs

Class LLParser :
   hashMap<NonTerminal*, hashMap<Terminal*, vector<Elem*>>> parsingTable;
   stack<Elem*> LLStack;
   vector<Terminal*> outputTerminals;
   hashMap<string, Terminal*> terminalsMapping;

Class LLParserGenerator:
   vector<Production*> rules;
   hashMap<NonTerminal*, unordered_set<Terminal*>> first, follow;
   hashMap<NonTerminal*, bool> isFirstBuild,isFollowBuild;
   hashMap<NonTerminal*, Production*> rulesMapping;
   NonTerminal* startState;

Class CFGBuilder :
   fstream rulesFile;
   vector<Production*> procList;
   hashMap<string, Production*> rulesMapping;
   hashMap<string, Terminal*> terminalsMapping;
   hashMap<string, NonTerminal*> nonTerminalsMapping;
```

### How to run
`
1.cd project_directory
2.run lex_rules.txt parser_rules.txt lab_program.txt
`
