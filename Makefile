CC = gcc
CFLAGS = -Wall -std=c99 

spellChkr: spellChkr.o
	$(CC) $(CFLAGS) -o spellChkr.c

clean:
	rm -f core $(EXE) *.o
