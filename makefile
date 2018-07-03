CC=gcc

all: main.o functions.o
		$(CC) main.o functions.o -pthread -Wall -o pipeline

functions.o: functions.c functions.h
		$(CC) functions.c -pthread -Wall -c -o functions.o

main.o: main.c
		$(CC) main.c -pthread -Wall -c -o main.o

clean:
	rm pipeline *.o output*.bmp
