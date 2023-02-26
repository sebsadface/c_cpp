#include "SocketUtil.h"

#include <unistd.h>
#include <assert.h>
#include <string.h>

#include <iostream>

using std::cerr;
using std::endl;

bool LookupName(char* name,
                unsigned short port,
                struct sockaddr_storage* ret_addr,
                size_t* ret_addrlen) {
  // TODO: fill in this function

}

bool Connect(const struct sockaddr_storage& addr,
             const size_t& addrlen,
             int* ret_fd) {
  // TODO: fill in this function

}

int WrappedRead(int fd, unsigned char* buf, int readlen) {
  // TODO: fill in this function

}

int WrappedWrite(int fd, unsigned char* buf, int writelen) {
  // TODO: fill in this function

}
