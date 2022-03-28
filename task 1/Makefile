CC = gcc


all: slast

slast : slast.o
	$(CC) slast.o -o slast

slast.o: slast.c slast.h
	$(CC) -c slast.c
	
.PHONY: clean all slast

clean: 
	rm -f *.o slast