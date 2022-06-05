CC = gcc

all: stree

stree: stree.o
	$(CC) stree.o -o stree

stree.o: stree.c stree.h
	$(CC) -c stree.c

.PHONY: clean all stree

clean:
	rm -f *.o stree
