CC=g++

all: sort

sort: main.o sort.o
	$(CC) -o sort main.o sort.o -std=c++14

main.o: main.cpp sort.h
	$(CC) -c main.cpp -std=c++14

sort.o: sort.cpp sort.h
	$(CC) -c sort.cpp -std=c++14

test: sort
	./sort

remove:
	rm -rf *.txt

clean:
	rm -rf *.o sort