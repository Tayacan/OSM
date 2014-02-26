CC = gcc
FLAGS = -pthread -g -Wall -Werror -pedantic -std=c99

RUN = $(CC) $(FLAGS)

matrix: stack.o

stack.o: stack.c stack.h
	$(RUN) -c stack.c


all: matrix

clean:
	rm -f *.o
	rm -f matrix