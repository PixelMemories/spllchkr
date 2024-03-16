CC = gcc
CFLAGS = -Wall -std=c99 

EXE = spellChkr

$(EXE): Makefile
	$(CC) $(CFLAGS) -o $@

clean:
	rm -f core $(EXE) *.o
