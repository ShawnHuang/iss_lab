CFLAGS	+= -g $(shell pkg-config --cflags --libs opencv)
CC = g++

all: exercise_1
.PHONY: all
exercise_1: exercise_1.o Split2hsv.o
	$(CC) exercise_1.o Split2hsv.o -o main.out -g $(CFLAGS)
exercise_1.o: exercise_1.cpp Split2hsv.hpp
	$(CC) -c exercise_1.cpp -g $(CFLAGS)
Split2hsv.o: Split2hsv.cpp Split2hsv.hpp
	$(CC) -c Split2hsv.cpp -g $(CFLAGS)

clean:
	rm *.out
	rm *.o
