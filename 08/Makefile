CC=g++

all: main.cpp ThreadPool.h
	$(CC) main.cpp -o threadPool  -std=c++11

test: ThreadPool.h
	./threadPool

clean:
	rm -rf *.o threadPool