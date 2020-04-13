CC=g++

all: main.cpp format.h
	$(CC) main.cpp -o format -std=c++11

test: format.h
	./format

clean:
	rm -rf *.o format