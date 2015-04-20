
CC = clang
CFLAGS = -std=c89
LDFLAGS = 


all: atom
run: all
	./atom

atom.o: atom.h atom.c
	$(CC) -c atom.c $(CFLAGS)

atom-main.o: atom.h atom-main.c
	$(CC) -c atom-main.c $(CFLAGS)

atom: atom-main.o atom.o
	$(CC) -o atom atom-main.o atom.o
