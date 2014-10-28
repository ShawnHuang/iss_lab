CFLAGS	+= -g $(shell pkg-config --cflags --libs opencv)
CC = g++

all: exercise_3
.PHONY: all
exercise_1: exercise_1.o Split2hsv.o
	$(CC) exercise_1.o Split2hsv.o -o main.out -g $(CFLAGS)
exercise_1.o: exercise_1.cpp Split2hsv.hpp
	$(CC) -c exercise_1.cpp -g $(CFLAGS)
Split2hsv.o: Split2hsv.cpp Split2hsv.hpp
	$(CC) -c Split2hsv.cpp -g $(CFLAGS)
exercise_2: exercise_2.o
	$(CC) exercise_2.o -o main.out -g $(CFLAGS)
exercise_2.o: exercise_2.cpp
	$(CC) -c exercise_2.cpp -g $(CFLAGS)
exercise_3: exercise_3.o
	$(CC) exercise_3.o -o main.out -g $(CFLAGS)
exercise_3.o: exercise_3.cpp
	$(CC) -std=c++0x -c exercise_3.cpp -g $(CFLAGS)
record: record.o
	$(CC) record.o -o main.out -g $(CFLAGS)
record.o: record.cpp
	$(CC) -std=c++0x -c record.cpp -g $(CFLAGS)
clean:
	rm *.out
	rm *.o
