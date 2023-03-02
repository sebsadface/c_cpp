#include "SocketUtil.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include <iostream>

using std::cerr;
using std::endl;

int Listen(char* portnum, int* sock_family) {
  // Create struct addrinfo hints.
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  // Set hints fields.
  hints.ai_family = AF_UNSPEC;      // allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // stream
  hints.ai_flags = AI_PASSIVE;      // use wildcard "in6addr_any" address
  hints.ai_protocol = IPPROTO_TCP;  // tcp protocol
  hints.ai_canonname = nullptr;
  hints.ai_addr = nullptr;
  hints.ai_next = nullptr;

  // Use argv[1] as the string representation of our portnumber to
  // pass in to getaddrinfo().  getaddrinfo() returns a list of
  // address structures via the output parameter "result".
  struct addrinfo* result;
  int res = getaddrinfo(nullptr, portnum, &hints, &result);

  // Did addrinfo() fail?
  if (res != 0) {
    std::cerr << "getaddrinfo() failed: ";
    std::cerr << gai_strerror(res) << std::endl;
    return -1;
  }

  // Loop through the returned address structures until we are able
  // to create a socket and bind to one.  The address structures are
  // linked in a list through the "ai_next" field of result.
  int listen_fd = -1;
  for (struct addrinfo* rp = result; rp != nullptr; rp = rp->ai_next) {
    listen_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (listen_fd == -1) {
      // Creating this socket failed.  So, loop to the next returned
      // result and try again.
      std::cerr << "socket() failed: " << strerror(errno) << std::endl;
      listen_fd = -1;
      continue;
    }

    // Configure the socket; we're setting a socket "option."  In
    // particular, we set "SO_REUSEADDR", which tells the TCP stack
    // so make the port we bind to available again as soon as we
    // exit, rather than waiting for a few tens of seconds to recycle it.
    int optval = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Try binding the socket to the address and port number returned
    // by getaddrinfo().
    if (bind(listen_fd, rp->ai_addr, rp->ai_addrlen) == 0) {
      // Bind worked!  Return to the caller the address family.
      *sock_family = rp->ai_family;
      break;
    }

    // The bind failed.  Close the socket, then loop back around and
    // try the next address/port returned by getaddrinfo().
    close(listen_fd);
    listen_fd = -1;
  }

  // Free the structure returned by getaddrinfo().
  freeaddrinfo(result);

  // Did we succeed in binding to any addresses?
  if (listen_fd == -1) {
    // No.  Return with failure.
    std::cerr << "Couldn't bind to any addresses." << std::endl;
    return listen_fd;
  }

  // Success. Tell the OS that we want this to be a listening socket.
  if (listen(listen_fd, SOMAXCONN) != 0) {
    std::cerr << "Failed to mark socket as listening: ";
    std::cerr << strerror(errno) << std::endl;
    close(listen_fd);
    return -1;
  }

  // Return to the client the listening file descriptor.
  return listen_fd;
}

int WrappedRead(int fd, unsigned char* buf, int readlen) {
  int result;
  while (1) {
    result = read(fd, buf, readlen);
    if (result == -1) {
      if ((errno == EAGAIN) || (errno == EINTR)) {
        // EINTR happened, so do nothing and try again
        continue;
      }
    }
    // A real error happened, so return an error result
    break;
  }
  return result;
}

int WrappedWrite(int fd, unsigned char* buf, int writelen) {
  int result;
  int totalwritten = 0;

  while (totalwritten < writelen) {
    result = write(fd, buf + totalwritten, writelen - totalwritten);
    if (result == -1) {
      if ((errno == EAGAIN) || (errno == EINTR)) {
        // EINTR happened, so do nothing and try again
        continue;
      }
      // A real error happened, so return an error result
      break;
    }
    if (result == 0) break;
    totalwritten += result;
  }
  return totalwritten;
}
