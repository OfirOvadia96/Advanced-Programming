CC = gcc

all: myshell

myshell: shell2.o
	$(CC) shell2.o -o myshell

shell2.o: shell2.c
	$(CC) -c shell2.c

.PHONY: clean all myshell

clean:
	rm -f *.o myshell