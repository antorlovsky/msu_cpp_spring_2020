CC=g++

all: parser

parser: main.o
	$(CC) -o parser main.o

main.o: main.cpp parser.h
	$(CC) -c main.cpp -std=c++11

test: parser
	./parser

clean:
	rm -rf *.o parser