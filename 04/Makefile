CC=g++

all: main.cpp serializer.h
	$(CC) main.cpp -o serializer -std=c++11

test: serializer.h
	./serializer

clean:
	rm -rf *.o serializer