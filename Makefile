CC = gcc
CFLAGS = -Wall

spellChkr:
	$(CC) $(CFLAGS) spellChkr.c -o spellChkr

clean:
	rm -rf *.o spellChkr
