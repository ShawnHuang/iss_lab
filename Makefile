CFLAGS	+= -g $(shell pkg-config --cflags --libs opencv)
CC = g++

all: exercise_5
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
exercise_4: exercise_4.o
	$(CC) exercise_4.o -o main.out -g $(CFLAGS)
exercise_4.o: exercise_4.cpp
	$(CC) -std=c++0x -c exercise_4.cpp -g $(CFLAGS)
exercise_5: exercise_5.o
	$(CC) exercise_5.o -o main.out -g $(CFLAGS)
exercise_5.o: exercise_5.cpp
	$(CC) -std=c++0x -c exercise_5.cpp -g $(CFLAGS)
exercise_6: exercise_6.o
	$(CC) exercise_6.o -o main.out -g $(CFLAGS)
exercise_6.o: exercise_6.cpp
	$(CC) -std=c++0x -c exercise_6.cpp -g $(CFLAGS)
record: record.o
	$(CC) record.o -o main.out -g $(CFLAGS)
record.o: record.cpp
	$(CC) -std=c++0x -c record.cpp -g $(CFLAGS)
clean:
	rm *.out
	rm *.o
