CC = g++ -std=c++11 -Wall -pedantic

all: trogger.x trogger.o

trogger.x: trogger.o
			$(CC) -o  trogger.x trogger.o -lncurses

trogger.o: trogger.cpp
	        $(CC) -c  trogger.cpp -lncurses
