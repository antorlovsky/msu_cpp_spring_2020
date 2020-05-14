CC=g++

all: sum

sum: main.o sum.o
	$(CC) -o sum main.o sum.o -std=c++11

main.o: main.cpp sum.h
	$(CC) -c main.cpp -std=c++11

sum.o: sum.cpp sum.h
	$(CC) -c sum.cpp -std=c++11

calc: sum
	./sum

clean:
	rm -rf *.o sum