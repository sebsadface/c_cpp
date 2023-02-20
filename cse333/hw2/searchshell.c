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

// Feature test macro for strtok_r (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "libhw1/CSE333.h"
#include "./CrawlFileTree.h"
#include "./DocTable.h"
#include "./MemIndex.h"

#define LINE_SIZE 8192  // size of the query buffer

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations, constants, etc
static void Usage(void);

// Get the queries, process the quries, and print the results
static void ProcessQueries(DocTable* dt, MemIndex* mi);

// Get the next line of query and parse it into words
// Returns -1 when eof reached, 0 when query is empty (no valid characters), and
// the length of the query array otherwise.
static int GetNextLine(FILE* f, char** ret_str);

//////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char** argv) {
  if (argc != 2) {
    Usage();
  }

  // Implement searchshell!  We're giving you very few hints
  // on how to do it, so you'll need to figure out an appropriate
  // decomposition into functions as well as implementing the
  // functions.  There are several major tasks you need to build:
  //
  //  - Crawl from a directory provided by argv[1] to produce and index
  //  - Prompt the user for a query and read the query from stdin, in a loop
  //  - Split a query into words (check out strtok_r)
  //  - Process a query against the index and print out the results
  //
  // When searchshell detects end-of-file on stdin (cntrl-D from the
  // keyboard), searchshell should free all dynamically allocated
  // memory and any other allocated resources and then exit.
  //
  // Note that you should make sure the fomatting of your
  // searchshell output exactly matches our solution binaries
  // to get full points on this part.

  MemIndex* mi;
  DocTable* dt;

  printf("Indexing '%s'\n", argv[1]);

  if (!CrawlFileTree(argv[1], &dt, &mi)) {
    // Crawling failed
    fprintf(stderr, "Path '%s' is not indexable\n", argv[1]);
    Usage();
  }

  // Get the queries, process the quries, and print the results
  ProcessQueries(dt, mi);

  // Cleaning up
  printf("shutting down...\n");
  MemIndex_Free(mi);
  DocTable_Free(dt);

  return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
// Helper function definitions

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative "
          "path to a directory to build an index under.\n");
  exit(EXIT_FAILURE);
}

static void ProcessQueries(DocTable* dt, MemIndex* mi) {
  LinkedList* ll;     // Result list
  LLIterator* iter;   // Result list iterator
  SearchResult* res;  // A single search result
  int qurey_len, i;
  char qurey[LINE_SIZE];  // Query array
  Verify333(qurey != NULL);

  qurey_len = GetNextLine(stdin, qurey);  // Get the fist line of qurey

  // Keep running until a eof is reached
  while (qurey_len != -1) {
    // Skip for next line of query if the current qurey is empty
    if (qurey_len > 0) {
      // Get the index for the query array
      ll = MemIndex_Search(mi, qurey, qurey_len);

      // Skip for next line of query if no matching documents were found
      if (ll != NULL) {
        iter = LLIterator_Allocate(ll);
        Verify333(iter != NULL);

        // Iterate through the result list and print out the results
        while (LLIterator_IsValid(iter)) {
          LLIterator_Get(iter, (LLPayload_t*)&res);
          printf("  %s (%d)\n", DocTable_GetDocName(dt, res->doc_id),
                 res->rank);
          LLIterator_Next(iter);
        }

        LLIterator_Free(iter);
        LinkedList_Free(ll, (LLPayloadFreeFnPtr)free);
      }
    }

    // Free the resources we allocated for each parsed words in the query
    for (i = 0; i < qurey_len; i++) {
      free(qurey[i]);
    }

    // Get a new line of query
    qurey_len = GetNextLine(stdin, qurey);
  }

  // EOF reached, clean the resources for query array
  free(qurey);
}

static int GetNextLine(FILE* f, char** ret_str) {
  char buffer[LINE_SIZE];
  char* token;  // A parsed token
  char* last;   // previously parsed string
  int i, ret_len, eof;
  ret_len = i = 0;

  printf("enter query:\n");

  if (fgets(buffer, sizeof(buffer), f) == NULL) {
    // Got a eof character (ctrl+d)
    return -1;
  }

  if (strlen(buffer) == 1) {
    // Got a empty line
    return 0;
  }

  // Eliminating tailing whitespace
  eof = (int)strlen(buffer);
  while (eof > 0 && isspace(buffer[eof - 1])) {
    eof--;
  }
  buffer[eof] = '\0';

  // Set all alphabets to lower case
  while (buffer[i] != '\0') {
    buffer[i] = (char)tolower((int)buffer[i]);
    i++;
  }

  // Parse the buffer into words and store them into the query array
  token = strtok_r(buffer, " ", &last);
  while (token != NULL) {
    ret_str[ret_len] = (char*)malloc(sizeof(char) * strlen(token) + 1);
    strncpy(ret_str[ret_len], token, strlen(token) + 1);
    ret_len++;  // Update the length of the query array
    token = strtok_r(NULL, " ", &last);
  }

  return ret_len;
}
