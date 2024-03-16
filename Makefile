CC = gcc
CFLAGS = 

spellChkr:
	$(CC) $(CFLAGS) spellChkr.c -o spellChkr

clean:
	rm -rf *.o spellChkr
