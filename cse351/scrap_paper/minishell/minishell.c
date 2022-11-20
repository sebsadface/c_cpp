#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARGS 100

/* Gets user input from stdin and then parses it into an array of strings
 * (argvp). A buffer is allocated (accessed via *bufferp) and the argvp
 * pointers point within this buffer.
 * The implementation can be found in get_input.c.
 */
void get_input(char** bufferp, unsigned long* buf_lenp, char** argvp);

int main() {
  char* buffer = NULL;
  unsigned long buf_len;
  char* new_argv[MAX_ARGS];

  while (1) {
    printf("> ");
    // read in user input into the char* array new_argv
    get_input(&buffer, &buf_len, new_argv);

    if (fork() == 0) {
      // A
    } else {
      // B
    }
  }
}

