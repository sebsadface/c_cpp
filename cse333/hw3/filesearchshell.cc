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

#include <cstdlib>   // for EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>  // for std::cout, std::cerr, etc.
#include <sstream>   // for stringstream

#include "./QueryProcessor.h"

using hw3::QueryProcessor;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

// Get a vector of query words input from the user
//
// Argument:
// - query: (output parameter) a vector of words input from the user. If the
// user input whitespaces or newline character, *query will be an empty vector.
//
// Returns:
// - false if EOF character is found, true otherwise
static bool GetQuery(vector<string>* const query);

// Print the result vector res to std out.
//
// Arguments:
// - res: a vector of query results, if res is empty, "[no results]" will be
// printed.
static void PrintResults(const vector<QueryProcessor::QueryResult>& res);

// Error usage message for the client to see
// Arguments:
// - prog_name: Name of the program
static void Usage(char* prog_name);

// Your job is to implement the entire filesearchshell.cc
// functionality. We're essentially giving you a blank screen to work
// with; you need to figure out an appropriate design, to decompose
// the problem into multiple functions or classes if that will help,
// to pick good interfaces to those functions/classes, and to make
// sure that you don't leak any memory.
//
// Here are the requirements for a working solution:
//
// The user must be able to run the program using a command like:
//
//   ./filesearchshell ./foo.idx ./bar/baz.idx /tmp/blah.idx [etc]
//
// i.e., to pass a set of filenames of indices as command line
// arguments. Then, your program needs to implement a loop where
// each loop iteration it:
//
//  (a) prints to the console a prompt telling the user to input the
//      next query.
//
//  (b) reads a white-space separated list of query words from
//      std::cin, converts them to lowercase, and constructs
//      a vector of c++ strings out of them.
//
//  (c) uses QueryProcessor.cc/.h's QueryProcessor class to
//      process the query against the indices and get back a set of
//      query results.  Note that you should instantiate a single
//      QueryProcessor  object for the lifetime of the program, rather
//      than  instantiating a new one for every query.
//
//  (d) print the query results to std::cout in the format shown in
//      the transcript on the hw3 web page.
//
// Also, you're required to quit out of the loop when std::cin
// experiences EOF, which a user passes by pressing "control-D"
// on the console.  As well, users should be able to type in an
// arbitrarily long query -- you shouldn't assume anything about
// a maximum line length.  Finally, when you break out of the
// loop and quit the program, you need to make sure you deallocate
// all dynamically allocated memory.  We will be running valgrind
// on your filesearchshell implementation to verify there are no
// leaks or errors.
//
// You might find the following technique useful, but you aren't
// required to use it if you have a different way of getting the
// job done.  To split a std::string into a vector of words, you
// can use a std::stringstream to get the job done and the ">>"
// operator. See, for example, "gnomed"'s post on stackoverflow for
// their example on how to do this:
//
//   http://stackoverflow.com/questions/236129/c-how-to-split-a-string
//
// (Search for "gnomed" on that page. They use an istringstream, but
// a stringstream gets the job done too.)
//
// Good luck, and write beautiful code!
int main(int argc, char** argv) {
  if (argc < 2) {
    Usage(argv[0]);
  }

  // STEP 1:
  // Implement filesearchshell!
  // Probably want to write some helper methods ...
  int i;
  list<string> idx_list;  // a list of index files for querying
  vector<string> query;   // a vector of query words
  vector<QueryProcessor::QueryResult> res;  // a vector of final query results

  // Push all the file paths from command line to the index file list
  for (i = 1; i < argc; i++) {
    idx_list.push_back(argv[i]);
  }

  // Initialize a query processor with the index file list.
  QueryProcessor qp(idx_list);

  while (1) {
    if (!GetQuery(&query)) {
      // Reached eof, exit the loop
      break;
    }

    // Check if the query is empty
    if (!query.empty()) {
      // Process the query
      res = qp.ProcessQuery(query);

      // Print out the results
      PrintResults(res);

      // clear res and query for reuse
      res.clear();
      query.clear();
    }
  }

  return EXIT_SUCCESS;
}

static bool GetQuery(vector<string>* const query) {
  string nextline;
  string nextword;
  stringstream ss;
  uint32_t i = 0;

  cout << "Enter query:" << endl;

  // Get a new line of user query from std in
  getline(cin, nextline);

  if (cin.eof()) {
    // Got a eof character (ctrl + d)
    return false;
  }

  // Check if the line of query only contains whitespaces
  for (char c : nextline) {
    if (isspace(c) && i == nextline.size() - 1) {
      return true;
    } else if (!isspace(c)) {
      break;
    }
    i++;
  }

  // Set all alphabets to lower case and store them to a string stream
  for (char c : nextline) {
    ss << static_cast<char>(tolower(c));
  }

  // Parse each word and push it to the query vector
  while (ss.good()) {
    ss >> nextword;
    if (!nextword.empty()) {
      query->push_back(nextword);
    }
  }
  return true;
}

static void PrintResults(const vector<QueryProcessor::QueryResult>& res) {
  if (res.empty()) {
    // No results found
    cout << "  [no results]" << endl;
  } else {
    // Print each result to std out
    for (auto qres : res) {
      cout << "  " << qres.document_name << " (" << qres.rank << ")" << endl;
    }
  }
}

static void Usage(char* prog_name) {
  cerr << "Usage: " << prog_name << " [index files+]" << endl;
  exit(EXIT_FAILURE);
}
