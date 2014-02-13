CC        =gcc
FLAGS     =-g -Wall -Werror -pedantic -std=c99

test: test.o dlist.o
	$(CC) $(FLAGS) test.o dlist.o -o test

test.o: test.c dlist.h
	$(CC) $(FLAGS) -c test.c

dlist.o: dlist.c dlist.h
	$(CC) $(FLAGS) -c dlist.c

all: test

run: test
	./test

clean:
	rm -f *.o
	rm -f test
