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

#define LINE_SIZE 256

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations, constants, etc
static void Usage(void);
static void ProcessQueries(DocTable* dt, MemIndex* mi);
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
    fprintf(stderr, "Path '%s' is not indexable\n", argv[1]);
    Usage();
  }

  ProcessQueries(dt, mi);

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
  LinkedList* res_list;
  SearchResult* res;
  char* query[LINE_SIZE];
  int query_len;

  query_len = GetNextLine(stdin, query);
  while (query_len != -1) {
    res_list = MemIndex_Search(mi, query, query_len);

    while (LinkedList_Pop(res_list, (LLPayload_t*)&res)) {
      printf("  %s (%d)\n", DocTable_GetDocName(dt, res->doc_id), res->rank);
    }

    query_len = GetNextLine(stdin, query);
  }
}

static int GetNextLine(FILE* f, char** ret_str) {
  char line[LINE_SIZE];
  char* saveptr;
  char* token;
  int query_len;
  printf("enter query:\n");
  fgets(line, LINE_SIZE, f);

  if (line == NULL) {
    return -1;
  }

  query_len = 0;
  token = strtok_r(line, " ", &saveptr);
  while (token != NULL) {
    ret_str[query_len] = token;
    query_len++;
    token = strtok_r(NULL, " ", &saveptr);
  }
  return query_len;  // you may want to change this
}
