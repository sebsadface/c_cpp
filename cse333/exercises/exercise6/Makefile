# Copyright © 2023 Sebatian Liu.
#
# Name: Sebastian Liu
# CSE Email Address: ll57@cs.washington.edu

CC = g++
CFLAGS = -Wall -g -std=c++17
PROGS = ex6 Vector.o

all: $(PROGS)

ex6: ex6.o Vector.o
	$(CC) $(CFLAGS) -o $@ $^

ex6.o: ex6.cc
	$(CC) $(CFLAGS) -c $@ $<

Vector.o: Vector.cc Vector.h
	$(CC) $(CFLAGS) -c $@ $<

clean:
	rm -f $(PROGS) *.o