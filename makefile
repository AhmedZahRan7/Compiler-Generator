CC = g++
CFLAGS = -Wall -g

all :=main.cpp 
all +=State.cpp
all +=Transation.cpp
all +=NFA.cpp
all +=utilities.cpp

outName = run

build: clean
	$(CC) $(CFLAGS) $(all) -o $(outName)
	run

clean:
	del -f *.o *.exe

run:
	$(outName).exe