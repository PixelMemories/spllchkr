CC = gcc
CFLAGS = -Wall -std=c99 

all:

spellChkr:
	gcc spellChkr.c -o spellChkr

clean:
	rm -f core $(EXE) *.o
