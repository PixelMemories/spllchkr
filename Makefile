CC = gcc
CFLAGS = -Wall -std=c99 

EXE = spellChkr

SRCS = spellChkr.c

OBJS = $(SRCS:.c=.o)

$(EXE): Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) 

clean:
	rm -f core $(EXE) *.o
