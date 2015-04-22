
CC = clang
CFLAGS = -std=c89
LDFLAGS = -lm


all: atom
run: all
	./atom

atom.o: atom.h atom.c
	$(CC) -c atom.c $(CFLAGS)

hash.o: hash.h hash.c
	$(CC) -c hash.c $(CFLAGS)

atom-main.o: hash.h atom.h atom-main.c
	$(CC) -c atom-main.c $(CFLAGS)

atom: atom-main.o atom.o hash.o
	$(CC) -o atom atom-main.o atom.o hash.o $(LDFLAGS)

.PHONY: clean
clean:
	rm -f atom atom.o atom-main.o hash.o
