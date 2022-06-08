CC = gcc

all: myshell

myshell: shell3.o
	$(CC) shell3.o -o myshell

shell2.o: shell3.c
	$(CC) -c shell3.c

.PHONY: clean all myshell

clean:
	rm -f *.o myshell