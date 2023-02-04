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

// Exit failure.
static void Usage(void);
// Print the filename and its rank.
static void PrintLinkedList(LinkedList *result, DocTable *doctable);

int main(int argc, char **argv) {
  // at least two arguments
  if (argc != 2) Usage();

  char *dir = argv[1];
  DocTable *doctable;
  MemIndex *index;

  // crawl from a directory provided by argv[1] to produce and index
  if (CrawlFileTree(dir, &doctable, &index) == 0) {
    fprintf(stderr, "Path \"%s \" is invalid.\n", argv[1]);
    Usage();
  }

  // Implement searchshell!  We're giving you very few hints
  // on how to do it, so you'll need to figure out an appropriate
  // decomposition into functions as well as implementing the
  // functions.  There are several major tasks you need to build:
  //
  //  - crawl from a directory provided by argv[1] to produce and index
  //  - prompt the user for a query and read the query from stdin, in a loop
  //  - split a query into words (check out strtok_r)
  //  - process a query against the index and print out the results
  //
  // When searchshell detects end-of-file on stdin (cntrl-D from the
  // keyboard), searchshell should free all dynamically allocated
  // memory and any other allocated resources and then exit.
  while (1) {
    char buffer[1024];
    printf("enter query:\n");
    // prompt the user for a query and read the query from stdin
    if (fgets(buffer, 1024, stdin) != NULL) {
      char **query = (char **)malloc(1024 * sizeof(char *));
      Verify333(query != NULL);

      uint8_t qlen = 0;
      char *token;
      int i = 0;
      char *rest = buffer;

      // convert to lower case
      while (buffer[i] != '\0') {
        buffer[i] = (char)tolower((int)buffer[i]);
        i++;
      }

      while (1) {
        token = strtok_r(rest, " ", &rest);   // split a query into words
        if (token == NULL || *token == '\n')  // reach the end
          break;
        query[qlen] = token;
        qlen++;
      }

      // assign the last character to be null terminator
      char *p = strchr(query[qlen - 1], '\n');
      if (p) *p = '\0';

      // process a query against the index
      LinkedList *result = MemIndex_Search(index, query, qlen);
      if (result == NULL) {
        free(query);
        continue;
      }

      PrintLinkedList(result, doctable);
      free(query);
      LinkedList_Free(result, &free);
    } else {  // EOF
      printf("\nshutting down...\n");
      break;
    }
  }
  DocTable_Free(doctable);
  MemIndex_Free(index);
  return EXIT_SUCCESS;
}

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative "
          "path to a directory to build an index under.\n");
  exit(EXIT_FAILURE);
}

static void PrintLinkedList(LinkedList *result, DocTable *doctable) {
  LLIterator *llit = LLIterator_Allocate(result);
  if (llit == NULL) {
    printf("No file is found.\n");
  }
  while (1) {
    SearchResult *sr;
    LLIterator_Get(llit, (LLPayload_t *)&sr);
    DocID_t docid = sr->doc_id;
    int rank = sr->rank;

    // find the directory name
    char *dir = DocTable_GetDocName(doctable, docid);
    Verify333(dir != NULL);
    printf("  %s (%d)\n", dir, rank);  // print out the results
    if (!LLIterator_Next(llit))        // reach the end
      break;
  }
  LLIterator_Free(llit);
}