CC=g++

all: parser

parser: main.o parser.o
	$(CC) -o parser main.o parser.o

main.o: main.cpp parser.h
	$(CC) -c main.cpp -std=c++11

parser.o: parser.cpp parser.h
	$(CC) -c parser.cpp -std=c++11

test: parser
	./parser

clean:
	rm -rf *.o parser