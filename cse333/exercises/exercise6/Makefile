CC = g++
CFLAGS = -Wall -g -std=c++17

all: ex6

ex6: ex6.cc Vector.cc Vector.h
	$(CC) $(CFLAGS) -o $@ $^

Vector.o: Vector.cc Vector.h
	$(CC) $(CFLAGS) -c $@ $<

ex6.o: ex6.cc Vector.cc Vector.h
	$(CC) $(CFLAGS) -c $@ $^

clean:
	rm -f $(PROGS) *.o