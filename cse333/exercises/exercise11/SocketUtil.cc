#include "SocketUtil.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include <iostream>

using std::cerr;
using std::endl;


int Listen(char* portnum, int* sock_family) {
  // TODO: fill in this function

}

int WrappedRead(int fd, unsigned char* buf, int readlen) {
  // TODO: copy this function from your Exercise 10 submission or from the
  // released sample solution
}

int WrappedWrite(int fd, unsigned char* buf, int writelen) {
  // TODO: copy this function from your Exercise 10 submission or from the
  // released sample solution
}
