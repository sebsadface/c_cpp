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

#define LINE_SIZE 8192

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
  LinkedList* ll;
  LLIterator* iter;
  SearchResult* res;
  int qurey_len;
  char** qurey = (char**)malloc(sizeof(char*) * LINE_SIZE);
  Verify333(qurey != NULL);

  qurey_len = GetNextLine(stdin, qurey);
  while (qurey_len != -1) {
    ll = MemIndex_Search(mi, qurey, qurey_len);
    if (ll != NULL) {
      iter = LLIterator_Allocate(ll);
      while (LLIterator_IsValid(iter)) {
        LLIterator_Get(iter, (LLPayload_t*)&res);
        printf("  %s (%d)\n", DocTable_GetDocName(dt, res->doc_id), res->rank);
        LLIterator_Next(iter);
      }
      LLIterator_Free(iter);
    }
    LinkedList_Free(ll, (LLPayloadFreeFnPtr)free);

    qurey_len = GetNextLine(stdin, qurey);
  }
  free(qurey);
}

static int GetNextLine(FILE* f, char** ret_str) {
  char buffer[LINE_SIZE];
  char* token;
  char* last;
  int i, ret_len;

  ret_len = i = 0;

  printf("enter query:\n");
  if (fgets(buffer, sizeof(buffer), f) == NULL) {
    return -1;
  }

  while (buffer[i] != '\n') {
    buffer[i] = (char)tolower((int)buffer[i]);
    i++;
  }
  printf("%d\n", strlen(buffer));
  token = strtok_r(buffer, " ", &last);
  while (token != NULL) {
    ret_str[ret_len] = token;
    ret_len++;
    token = strtok_r(NULL, " ", &last);
  }
  *strchr(ret_str[ret_len - 1], '\n') = '\0';

  return ret_len;  // you may want to change this
}
