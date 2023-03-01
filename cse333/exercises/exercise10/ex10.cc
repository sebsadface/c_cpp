// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include <fcntl.h>   // for open(), O_RDONLY
#include <stdio.h>   // for printf()
#include <stdlib.h>  // for exit()
#include <iostream>  // for cerr, endl
#include <unistd.h>  // for close(), read(), write()

#include "./SocketUtil.h"

#define BUFFER_SIZE 256

using std::cerr;
using std::endl;

void Usage();

// Open a local file, open a socket to a remote process,
// and send the file to other process.
int main(int argc, char **argv) {
  // TODO: fill in this main method.
  struct sockaddr_storage addr;
  size_t addrlen;
  int socket_fd;
  int file_fd;
  unsigned short port = 0;
  unsigned char readbuf[BUFFER_SIZE];

  // check the number of arguments
  if (argc != 4) {
    Usage();
  }

  // Open the file
  file_fd = open(argv[3], O_RDONLY);
  if (file_fd == -1) {
    Usage();
  }

  // Get port number.
  if (sscanf(argv[2], "%hu", &port) != 1) {
    Usage();
  }

  // Get sockaddr structure.
  if (!LookupName(argv[1], port, &addr, &addrlen)) {
    Usage();
  }

  // Connect to the server.
  if (!Connect(addr, addrlen, &socket_fd)) {
    Usage();
  }

  while (1) {
    // Read from the file.
    int res = WrappedRead(file_fd, readbuf, BUFFER_SIZE);
    if (res <= 0) {
      // EOF or error.
      break;
    }

    // Write to the socket.
    int res2 = WrappedWrite(socket_fd, readbuf, res);
    if (res2 != res) {
      // Error.
      break;
    }
  }

  // Clean up.
  close(socket_fd);
  close(file_fd);
  return EXIT_SUCCESS;
}

void Usage() {
  cerr << "usage: "
       << " <HOSTNAME> <PORT NUMBER> <FILENAME>" << endl;
  exit(EXIT_FAILURE);
}
