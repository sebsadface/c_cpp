// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include "SocketUtil.h"

#include <unistd.h>  // for open(), close(), read(), write()
#include <string.h>  // for memset(), memcpy()

#include <iostream>

using std::cerr;
using std::endl;

bool LookupName(char* name, unsigned short port,
                struct sockaddr_storage* ret_addr, size_t* ret_addrlen) {
  // Step1: Create struct addrinfo hints.
  struct addrinfo hints, *results;
  int retval;

  // Step2: Zero out the hints data structure using memset.
  memset(&hints, 0, sizeof(hints));

  // Step3: Set fields of hints
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  // Step4: Call getaddrinfo() using &hints.
  if ((retval = getaddrinfo(name, NULL, &hints, &results)) != 0) {
    std::cerr << "getaddrinfo failed: ";
    std::cerr << gai_strerror(retval) << std::endl;
    return false;
  }

  // Step5: Resulting linked list *results will have all fields approbroately
  // set. We need to treat the IPv4 and IPv6 cases differently.
  if (results->ai_family == AF_INET) {
    struct sockaddr_in* v4addr = (struct sockaddr_in*)results->ai_addr;
    v4addr->sin_port = htons(port);
  } else if (results->ai_family == AF_INET6) {
    struct sockaddr_in6* v6addr = (struct sockaddr_in6*)results->ai_addr;
    v6addr->sin6_port = htons(port);
  } else {
    std::cerr << "getaddrinfo failed giving IPv4 or IPv6 address";
    std::cerr << std::endl;
    return false;
  }

  // Get the first result and copy it into ret_addr.
  memcpy(ret_addr, results->ai_addr, results->ai_addrlen);
  *ret_addrlen = results->ai_addrlen;

  // Clean up.
  freeaddrinfo(results);
  return true;
}

bool Connect(const struct sockaddr_storage& addr, const size_t& addrlen,
             int* ret_fd) {
  // Step1: We've figureed out the IP address and port to connect to.

  // Step2: Create a socket using socket().
  int socket_fd = socket(addr.ss_family, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    std::cerr << "socket() failed: " << strerror(errno) << std::endl;
    return false;
  }

  // Step3: Connect to the server using connect().
  int res =
      connect(socket_fd, reinterpret_cast<const sockaddr*>(&addr), addrlen);
  if (res == -1) {
    std::cerr << "connect() failed: " << strerror(errno) << std::endl;
    return false;
  }

  *ret_fd = socket_fd;
  return true;
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
