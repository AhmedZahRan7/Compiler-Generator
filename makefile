CC = g++
CFLAGS = -Wall -g

all :=main.cpp
all +=DFA.cpp
all +=Lexical_Analyzer_Generator.cpp
all +=Lexical_Analyzer.cpp
all +=NFA.cpp
all +=NFABuilder.cpp
all +=State.cpp
all +=Token.cpp
all +=TokenKey.cpp
all +=TokenValue.cpp
all +=Transation.cpp
all +=utilities.cpp
outName = run

build: clean
	$(CC) $(CFLAGS) $(all) -o $(outName)
	run

clean:
	del -f *.o *.exe

run:
	$(outName).exe
