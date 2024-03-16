CC = gcc
CFLAGS = -Wall -std=c99 

spellChkr:
	$(CC) $(CFLAGS) spellChkr.c -o spellChkr

clean:
	rm -f core $(EXE) *.o
