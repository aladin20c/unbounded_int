#calc_unbounded_int.c
#test_unbounded.c

CC=gcc
CFLAGS= -Wall -pedantic -g

all : calc_unbounded_int test_unbounded


test_unbounded : test_unbounded.o
	$(CC) -o test_unbounded test_unbounded.o

calc_unbounded_int : calc_unbounded_int.o
	$(CC) -o calc_unbounded_int calc_unbounded_int.o

unbounded_int.o : unbounded_int.c unbounded_int.h
calc_unbounded_int.o : calc_unbounded_int.c unbounded_int.o
test_unbounded.o : test_unbounded.c unbounded_int.o

clean :
		rm -rf *.o
		rm -rf a.out
		rm -rf test_unbounded
		rm -rf calc_unbounded_int