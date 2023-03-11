/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>       // for snprintf()
#include <unistd.h>      // for close(), fcntl()
#include <sys/types.h>   // for socket(), getaddrinfo(), etc.
#include <sys/socket.h>  // for socket(), getaddrinfo(), etc.
#include <arpa/inet.h>   // for inet_ntop()
#include <netdb.h>       // for getaddrinfo()
#include <errno.h>       // for errno, used by strerror()
#include <string.h>      // for memset, strerror()
#include <iostream>      // for std::cerr, etc.

#include "./ServerSocket.h"

extern "C" {
#include "libhw1/CSE333.h"
}

using std::string;
using std::to_string;

namespace hw4 {
static const int kBufferLength = 1024;

ServerSocket::ServerSocket(uint16_t port) {
  port_ = port;
  listen_sock_fd_ = -1;
}

ServerSocket::~ServerSocket() {
  // Close the listening socket if it's not zero.  The rest of this
  // class will make sure to zero out the socket if it is closed
  // elsewhere.
  if (listen_sock_fd_ != -1) close(listen_sock_fd_);
  listen_sock_fd_ = -1;
}

bool ServerSocket::BindAndListen(int ai_family, int* const listen_fd) {
  // Use "getaddrinfo," "socket," "bind," and "listen" to
  // create a listening socket on port port_.  Return the
  // listening socket through the output parameter "listen_fd"
  // and set the ServerSocket data member "listen_sock_fd_"

  // STEP 1:

  // Check if the ai_family is valid
  if (ai_family != AF_INET && ai_family != AF_INET6 && ai_family != AF_UNSPEC) {
    return false;
  }

  // Populate the "hints" addrinfo structure for getaddrinfo().
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = ai_family;      // given ai_family
  hints.ai_socktype = SOCK_STREAM;  // stream
  hints.ai_flags = AI_PASSIVE;      // use wildcard "INADDR_ANY"
  hints.ai_flags |= AI_V4MAPPED;    // use v4-mapped v6 if no v6 found
  hints.ai_protocol = IPPROTO_TCP;  // tcp protocol
  hints.ai_canonname = nullptr;
  hints.ai_addr = nullptr;
  hints.ai_next = nullptr;

  // Get a  string representation of portnumber to
  //  pass in to getaddrinfo().  getaddrinfo() returns a list of
  //  address structures via the output parameter "result".
  struct addrinfo* result;
  string portnum = to_string(this->port_);
  int res = getaddrinfo(nullptr, portnum.c_str(), &hints, &result);

  // Did addrinfo() fail?
  if (res != 0) {
    return false;
  }

  // Loop through the returned address structures until we are able
  // to create a socket and bind to one.  The address structures are
  // linked in a list through the "ai_next" field of result.
  for (struct addrinfo* rp = result; rp != nullptr; rp = rp->ai_next) {
    this->listen_sock_fd_ =
        socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

    if (this->listen_sock_fd_ == -1) {
      // Creating this socket failed.  So, loop to the next returned
      // result and try again.
      this->listen_sock_fd_ = -1;
      continue;
    }

    // Configure the socket; we're setting a socket "option."  In
    // particular, we set "SO_REUSEADDR", which tells the TCP stack
    // so make the port we bind to available again as soon as we
    // exit, rather than waiting for a few tens of seconds to recycle it.
    int optval = 1;
    setsockopt(this->listen_sock_fd_, SOL_SOCKET, SO_REUSEADDR, &optval,
               sizeof(optval));

    // Try binding the socket to the address and port number returned
    // by getaddrinfo().
    if (bind(this->listen_sock_fd_, rp->ai_addr, rp->ai_addrlen) == 0) {
      // Bind worked!  Return to the caller the address family.
      this->sock_family_ = rp->ai_family;
      break;
    }

    // The bind failed.  Close the socket, then loop back around and
    // try the next address/port returned by getaddrinfo().
    close(this->listen_sock_fd_);
    this->listen_sock_fd_ = -1;
  }

  // Free the structure returned by getaddrinfo().
  freeaddrinfo(result);

  // If we failed to bind, return failure.
  if (this->listen_sock_fd_ <= 0) {
    return false;
  }

  // Success. Tell the OS that we want this to be a listening socket.
  if (listen(this->listen_sock_fd_, SOMAXCONN) != 0) {
    // Failed to mark socket as listening
    close(this->listen_sock_fd_);
    return false;
  }

  // Return to the client the listening file descriptor.
  *listen_fd = this->listen_sock_fd_;
  return true;
}

bool ServerSocket::Accept(int* const accepted_fd,
                          std::string* const client_addr,
                          uint16_t* const client_port,
                          std::string* const client_dns_name,
                          std::string* const server_addr,
                          std::string* const server_dns_name) const {
  // Accept a new connection on the listening socket listen_sock_fd_.
  // (Block until a new connection arrives.)  Return the newly accepted
  // socket, as well as information about both ends of the new connection,
  // through the various output parameters.

  // STEP 2:
  int client_fd;
  struct sockaddr_storage caddr;
  socklen_t caddr_len = sizeof(caddr);

  // Loop forever, accepting a connection from a client.
  while (1) {
    client_fd = accept(this->listen_sock_fd_,
                       reinterpret_cast<struct sockaddr*>(&caddr), &caddr_len);
    if (client_fd < 0) {
      if ((errno == EAGAIN) || (errno == EINTR)) {
        continue;
      }
      // Accepting connection failed clean up and return false
      close(this->listen_sock_fd_);
      return false;
    }
    // Accepting connection sucseeded break out of the loop
    break;
  }

  // Return the client socket
  *accepted_fd = client_fd;

  // Check client's ai_family
  if (caddr.ss_family == AF_INET) {
    // We have a IPV4 address
    char astring[INET_ADDRSTRLEN];

    // Get the IPV4 socket address
    struct sockaddr_in* sa4 = reinterpret_cast<struct sockaddr_in*>(&caddr);
    inet_ntop(AF_INET, &(sa4->sin_addr), astring, INET_ADDRSTRLEN);
    *client_addr = string(astring);
    *client_port = htons(sa4->sin_port);
  } else {
    // We have a IPV6 address
    char astring[INET6_ADDRSTRLEN];

    // Get the IPV6 socket address
    struct sockaddr_in6* sa6 = reinterpret_cast<struct sockaddr_in6*>(&caddr);
    inet_ntop(AF_INET6, &(sa6->sin6_addr), astring, INET6_ADDRSTRLEN);
    *client_addr = string(astring);
    *client_port = htons(sa6->sin6_port);
  }

  // Get client's dns name
  char chname[kBufferLength];
  chname[0] = '\0';
  getnameinfo(reinterpret_cast<struct sockaddr*>(&caddr), caddr_len, chname,
              kBufferLength, NULL, 0, 0);
  *client_dns_name = string(chname);

  // Get the server's addr and dns name
  char shname[kBufferLength];
  shname[0] = '\0';
  if (sock_family_ == AF_INET) {
    // The server is IPV4

    struct sockaddr_in srvr;
    socklen_t srvrlen = sizeof(srvr);
    char addrbuf[INET_ADDRSTRLEN];
    getsockname(client_fd, reinterpret_cast<struct sockaddr*>(&srvr), &srvrlen);
    inet_ntop(AF_INET, &srvr.sin_addr, addrbuf, INET_ADDRSTRLEN);
    getnameinfo(reinterpret_cast<struct sockaddr*>(&srvr), srvrlen, shname,
                kBufferLength, nullptr, 0, 0);

    *server_addr = string(addrbuf);
    *server_dns_name = string(shname);
  } else {
    // The server is IPV6

    struct sockaddr_in6 srvr;
    socklen_t srvrlen = sizeof(srvr);
    char addrbuf[INET6_ADDRSTRLEN];
    getsockname(client_fd, reinterpret_cast<struct sockaddr*>(&srvr), &srvrlen);
    inet_ntop(AF_INET6, &srvr.sin6_addr, addrbuf, INET6_ADDRSTRLEN);
    getnameinfo(reinterpret_cast<struct sockaddr*>(&srvr), srvrlen, shname,
                kBufferLength, nullptr, 0, 0);

    *server_addr = string(addrbuf);
    *server_dns_name = string(shname);
  }

  return true;
}

}  // namespace hw4
