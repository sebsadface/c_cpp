#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void get_input(char** bufferp, unsigned long* buf_lenp, char** argvp) {
  int num_read;
  // Reads the characters of the user input into a newly-allocated buffer
  // *bufferp of length *buf_lenp.
  // For more info, see https://man7.org/linux/man-pages/man3/getline.3.html
  num_read = getline(bufferp, buf_lenp, stdin);
  if (num_read == -1) {
    // Error
    perror("Error reading input: ");
    exit(EXIT_FAILURE);
  }

  // Tokenize by splitting input on spaces and newlines and storing into the
  // character pointer array argvp.
  // For more info, see https://cplusplus.com/reference/cstring/strtok/
  char delim[] = " \n";
  char* ptr = strtok(*bufferp, delim);
  int num_words = 0;
  argvp[num_words] = ptr;
  num_words++;

  while (ptr != NULL) {
    ptr = strtok(NULL, delim);
    argvp[num_words] = ptr;
    num_words++;
  }
  argvp[num_words] = NULL;
}
