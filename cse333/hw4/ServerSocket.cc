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
  if (ai_family != AF_INET && ai_family != AF_INET6 && ai_family != AF_UNSPEC) {
    return false;
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = ai_family;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_flags |= AI_V4MAPPED;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_canonname = nullptr;
  hints.ai_addr = nullptr;
  hints.ai_next = nullptr;

  struct addrinfo* result;
  string portnum = to_string(this->port_);

  int res = getaddrinfo(nullptr, portnum.c_str(), &hints, &result);

  if (res != 0) {
    return false;
  }

  for (struct addrinfo* rp = result; rp != nullptr; rp = rp->ai_next) {
    this->listen_sock_fd_ =
        socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

    if (this->listen_sock_fd_ == -1) {
      this->listen_sock_fd_ = -1;
      continue;
    }

    int optval = 1;
    setsockopt(this->listen_sock_fd_, SOL_SOCKET, SO_REUSEADDR, &optval,
               sizeof(optval));

    if (bind(this->listen_sock_fd_, rp->ai_addr, rp->ai_addrlen) == 0) {
      this->sock_family_ = rp->ai_family;
      break;
    }

    close(this->listen_sock_fd_);
    this->listen_sock_fd_ = -1;
  }

  freeaddrinfo(result);

  if (this->listen_sock_fd_ <= 0) {
    return false;
  }

  if (listen(this->listen_sock_fd_, SOMAXCONN) != 0) {
    close(this->listen_sock_fd_);
    return false;
  }

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
  while (1) {
    client_fd = accept(this->listen_sock_fd_,
                       reinterpret_cast<struct sockaddr*>(&caddr), &caddr_len);
    if (client_fd < 0) {
      if ((errno == EAGAIN) || (errno == EINTR)) {
        continue;
      }
      close(this->listen_sock_fd_);
      return false;
    }
    break;
  }

  *accepted_fd = client_fd;

  if (caddr.ss_family == AF_INET) {
    char astring[INET_ADDRSTRLEN];

    struct sockaddr_in* sa4 = reinterpret_cast<struct sockaddr_in*>(&caddr);
    inet_ntop(AF_INET, &(sa4->sin_addr), astring, INET_ADDRSTRLEN);
    *client_addr = string(astring);
    *client_port = htons(sa4->sin_port);
  } else {
    char astring[INET6_ADDRSTRLEN];

    struct sockaddr_in6* sa6 = reinterpret_cast<struct sockaddr_in6*>(&caddr);
    inet_ntop(AF_INET6, &(sa6->sin6_addr), astring, INET6_ADDRSTRLEN);
    *client_addr = string(astring);
    *client_port = htons(sa6->sin6_port);
  }

  char hname[1024];
  hname[0] = '\0';
  if (sock_family_ == AF_INET) {
    struct sockaddr_in srvr;
    socklen_t srvrlen = sizeof(srvr);
    char addrbuf[INET_ADDRSTRLEN];
    getsockname(client_fd, reinterpret_cast<struct sockaddr*>(&srvr), &srvrlen);
    inet_ntop(AF_INET, &srvr.sin_addr, addrbuf, INET_ADDRSTRLEN);
    getnameinfo(reinterpret_cast<struct sockaddr*>(&srvr), srvrlen, hname, 1024,
                nullptr, 0, 0);

    *server_addr = string(addrbuf);
    *server_dns_name = string(hname);
  } else {
    struct sockaddr_in6 srvr;
    socklen_t srvrlen = sizeof(srvr);
    char addrbuf[INET6_ADDRSTRLEN];
    getsockname(client_fd, reinterpret_cast<struct sockaddr*>(&srvr), &srvrlen);
    inet_ntop(AF_INET6, &srvr.sin6_addr, addrbuf, INET6_ADDRSTRLEN);
    getnameinfo(reinterpret_cast<struct sockaddr*>(&srvr), srvrlen, hname, 1024,
                nullptr, 0, 0);

    *server_addr = string(addrbuf);
    *server_dns_name = string(hname);
  }

  return true;
}

}  // namespace hw4
