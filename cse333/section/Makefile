# CSE 333 Makefile for Section 2 (wordcount.c)

CC = gcc
CFLAGS = -std=c17 -g -Wall
EXEC = wordcount

# Compiles wordcount; this is the default rule that fires if a user
# just types "make" in the same directory as this Makefile.
wordcount: $(EXEC).c
	$(CC) $(CFLAGS) -o $(EXEC) $(EXEC).c

val:
	valgrind --leak-check=full ./$(EXEC)

clean:
	rm -f *.o *~ $(EXEC)
