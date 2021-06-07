LEX_DIR := Lexical_Analyzer_Generator
LEX_SRC := $(wildcard $(LEX_DIR)/*.cpp)
PARSER_DIR := Parser_Generator
PARSER_SRC := $(wildcard $(PARSER_DIR)/*.cpp)

CC = g++
CFLAGS = -Wall -g -I$(LEX_DIR) -I$(PARSER_DIR)

all := main.cpp
all += $(LEX_SRC)
all += $(PARSER_SRC)
outName = run

build:clean
	$(CC) $(CFLAGS) $(all) -o $(outName)

clean:
	del -f *.o *.exe

run:
	$(outName).exe
