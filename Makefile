CC = gcc
CFLAGS = -Wall -std=c99 

EXE = spellChkr

spellChkr: spellChkr.o
	$(CC) $(CFLAGS) -o EXE

clean:
	rm -f core $(EXE) *.o
