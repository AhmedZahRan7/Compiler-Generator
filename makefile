LEX_DIR := Lexical_Analyzer_Generator
LEX_SRC := $(wildcard $(LEX_DIR)/*.cpp)
PARSER_DIR := Parser_Generator
# PARSER_SRC := $(wildcard $(PARSER_DIR)/*.cpp)
PARSER_SRC := $(PARSER_DIR)/CFGBuilder.cpp
PARSER_SRC += $(PARSER_DIR)/Elem.cpp
PARSER_SRC += $(PARSER_DIR)/Production.cpp
PARSER_SRC += $(PARSER_DIR)/NonTerminal.cpp
PARSER_SRC += $(PARSER_DIR)/Terminal.cpp


CC = g++
CFLAGS = -Wall -g -I$(LEX_DIR) -I$(PARSER_DIR)

#all := main.cpp
all := test.cpp
all += $(LEX_SRC)
all += $(PARSER_SRC)
outName = run

build: clean
	$(CC) $(CFLAGS) $(all) -o $(outName)

clean:
	del -f *.o *.exe

run:
	$(outName).exe
