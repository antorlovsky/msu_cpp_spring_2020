CC=g++

all: matrix

matrix: main.o Matrix.o
	$(CC) -o matrix main.o Matrix.o

main.o: main.cpp Matrix.h
	$(CC) -c main.cpp

Matrix.o: Matrix.cpp Matrix.h
	$(CC) -c Matrix.cpp

test: matrix
	./matrix

clean:
	rm -rf *.o matrix