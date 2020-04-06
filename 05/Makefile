CC=g++

all: BigInt

BigInt: main.o BigInt.o
	$(CC) -o BigInt main.o BigInt.o

main.o: main.cpp BigInt.h
	$(CC) -c main.cpp -std=c++11

BigInt.o: BigInt.cpp BigInt.h
	$(CC) -c BigInt.cpp -std=c++11

test: BigInt
	./BigInt

clean:
	rm -rf *.o BigInt