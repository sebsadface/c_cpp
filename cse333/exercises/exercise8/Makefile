# Copyright © 2023 Sebatian Liu.
#
# Name: Sebastian Liu
# CSE Email Address: ll57@cs.washington.edu

CC = g++
CFLAGS = -Wall -g -std=c++17
OBJS = ex8.o
EXE = ex8

#default target
$(EXE): $(OBJS)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

# Source file
%.o: %.CC
		$(CC) $(CFLAGS) -c $<

# phony target - delete built files (including OS X debug files)
clean:
        rm -f $(EXE) *.o *~ *.dSYM