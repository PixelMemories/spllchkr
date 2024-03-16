CC = gcc
CFLAGS = -Wall -std=c99 

spellChkr: spellChkr.o
	$(CC) $(CFLAGS) -o spellChkr

clean:
	rm -f core $(EXE) *.o
