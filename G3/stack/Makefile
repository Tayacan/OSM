CC = gcc
FLAGS = -g -Wall -Werror -pedantic -std=gnu99
FLAGS2 = -lrt -lpthread

RUN = $(CC) $(FLAGS)

matrix: matrix.o stack.o
	$(RUN) matrix.o stack.o -o matrix $(FLAGS2)

matrix.o: matrix.c stack.h
	$(RUN) -c matrix.c $(FLAGS2)

stack.o: stack.c stack.h
	$(RUN) -c stack.c $(FLAGS2)


all: matrix

clean:
	rm -f *.o
	rm -f matrix
