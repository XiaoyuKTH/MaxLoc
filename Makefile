CC = gcc
CFlags = -fopenmp

all: MaxLoc

MaxLoc: MaxLoc.c
	$(CC) $(CFlags) MaxLoc.c -o $@

clean:
	@rm -rf MaxLoc
