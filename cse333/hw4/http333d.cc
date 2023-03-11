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

#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <list>

#include "./ServerSocket.h"
#include "./HttpServer.h"

using std::cerr;
using std::cout;
using std::endl;
using std::list;
using std::string;

// Print out program usage, and exit() with EXIT_FAILURE.
static void Usage(char* prog_name);

// Parse command-line arguments to get port, path, and indices to use
// for your http333d server.
//
// Params:
// - argc: number of argumnets
// - argv: array of arguments
// - port: output parameter returning the port number to listen on
// - path: output parameter returning the directory with our static files
// - indices: output parameter returning the list of index file names
//
// Calls Usage() on failure. Possible errors include:
// - path is not a readable directory
// - index file names are readable
static void GetPortAndPath(int argc, char** argv, uint16_t* const port,
                           string* const path, list<string>* const indices);

int main(int argc, char** argv) {
  // Print out welcome message.
  cout << "Welcome to http333d, the UW cse333 web server!" << endl;
  cout << "  Copyright 2012 Steven Gribble" << endl;
  cout << "  http://www.cs.washington.edu/homes/gribble" << endl;
  cout << endl;
  cout << "initializing:" << endl;
  cout << "  parsing port number and static files directory..." << endl;

  // Ignore the SIGPIPE signal, otherwise we'll crash out if a client
  // disconnects unexpectedly.
  signal(SIGPIPE, SIG_IGN);

  // Get the port number and list of index files.
  uint16_t port_num;
  string static_dir;
  list<string> indices;
  GetPortAndPath(argc, argv, &port_num, &static_dir, &indices);
  cout << "    port: " << port_num << endl;
  cout << "    path: " << static_dir << endl;

  // Run the server.
  hw4::HttpServer hs(port_num, static_dir, indices);
  if (!hs.Run()) {
    cerr << "  server failed to run!?" << endl;
  }

  cout << "server completed!  Exiting." << endl;
  return EXIT_SUCCESS;
}

static void Usage(char* prog_name) {
  cerr << "Usage: " << prog_name << " port staticfiles_directory indices+";
  cerr << endl;
  exit(EXIT_FAILURE);
}

static void GetPortAndPath(int argc, char** argv, uint16_t* const port,
                           string* const path, list<string>* const indices) {
  // Here are some considerations when implementing this function:
  // - There is a reasonable number of command line arguments
  // - The port number is reasonable
  // - The path (i.e., argv[2]) is a readable directory
  // - You have at least 1 index, and all indices are readable files

  // STEP 1:

  // Check for reasonable number of command line arguments
  if (argc < 4) {
    Usage(argv[0]);
  }

  // Check to see if the port number is reasonable
  *port = atoi(argv[1]);
  if (*port < 1024 || *port > 65535) {
    cerr << "Unreasonable port number, reasonable range: 1024 <= port_number "
            "<= 65535"
         << endl;
    Usage(argv[0]);
  }

  // Check if the path is a readable directory
  *path = string(argv[2]);
  struct stat dir_stat;
  if (stat(argv[2], &dir_stat) == -1 || !S_ISDIR(dir_stat.st_mode)) {
    cerr << *path << "is not a readable directory" << endl;
    Usage(argv[0]);
  }

  // Check all indices are readable, add them to indices list if they are
  for (int i = 3; i < argc; i++) {
    string file_name = string(argv[i]);

    if (file_name.length() >= 4 &&
        file_name.substr(file_name.length() - 4) == ".idx") {
      struct stat file_stat;
      if (stat(argv[i], &file_stat) == -1 || !S_ISREG(file_stat.st_mode)) {
        cerr << file_name << "is not a readable file" << endl;
        Usage(argv[0]);
      }
      indices->push_back(file_name);
    }
  }

  // Check if we have at least one readable indice file
  if (indices->size() == 0) {
    cerr << "Need at least one index file!" << endl;
    Usage(argv[0]);
  }
}
