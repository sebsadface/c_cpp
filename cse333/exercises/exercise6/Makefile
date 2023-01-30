# Copyright © 2023 Sebatian Liu.
#
# Name: Sebastian Liu
# CSE Email Address: ll57@cs.washington.edu

CC = g++
CFLAGS = -Wall -g -std=c++17

# default target
ex6: ex6.o Vector.o
	$(CC) $(CFLAGS) -o $@ $^

# individual source files
ex6.o: ex6.cc Vector.h
	$(CC) $(CFLAGS) -c $<

Vector.o: Vector.cc Vector.h
	$(CC) $(CFLAGS) -c $<

# phony target - delete built files (including OS X debug files)
clean:
	rm -f ex6 *.o *~ ex6.dSYM