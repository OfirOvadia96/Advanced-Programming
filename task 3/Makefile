CC = gcc

all: myshell

myshell: shell.o
	$(CC) shell.o -o myshell

shell2.o: shell3.c
	$(CC) -c shell.c

.PHONY: clean all myshell

clean:
	rm -f *.o myshell
