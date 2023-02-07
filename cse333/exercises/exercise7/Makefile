# Copyright © 2023 Sebatian Liu.
#
# Name: Sebastian Liu
# CSE Email Address: ll57@cs.washington.edu

CC = g++
CFLAGS = -Wall -g -std=c++17
OBJS = Vector.o ex7.o
EXE = ex7
HEADER = Vector.h

# default target
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $<

# individual source files
# `%` is a makefile internal wildcard which will serve as a 
# placeholder in a for-each manner
%.o: %.cc $(HEADER)
	$(CC) $(CFLAGS) -c $<

# phony target - delete built files (including OS X debug files)
clean:
	rm -f $(EXE) *.o *~ *.dSYM