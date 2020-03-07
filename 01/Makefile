CC=g++

all: allocator

allocator: main.o alloc.o
	$(CC) -o allocator main.o alloc.o

main.o: main.cpp alloc.h
	$(CC) -c main.cpp

alloc.o: alloc.cpp alloc.h
	$(CC) -c alloc.cpp

test: allocator
	./allocator

clean:
	rm -rf *.o allocator