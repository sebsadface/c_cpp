// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include <stdio.h>    // for snprintf
#include <stdlib.h>   // for EXIT_SUCCESS, NULL
#include <string.h>   // for strrchr, strcmp, strlen
#include <stdbool.h>  // for bool
#include <dirent.h>   // DIR

#include "ro_file.h"

#define READBUFSIZE 1024
/*** HELPER FUNCTION DECLARATIONS ******************************************/

// Returns whether or not a filename ends in ".txt".
bool IsTxtFile(char* filename);

// Concatenate the directory and file names into a full path. The caller is
// responsible for freeing the allocated string. Exits if an error occurs.
char* Concatenate(char* dirname, char* filename);

/*
 * This program:
 * - Accepts a directory name as a command-line argument.
 * - Scans through the directory looking for all files whose names end with
 *   the four characters ".txt".
 * - For every such file found, write the contents of those files to stdout
 *   without adding any additional characters or formatting.
 *   Eventually reading the files with ro_file module.
 */
int main(int argc, char** argv) {
  // TODO: Write this function
  if (argc != 2) {
    fprintf(stderr,
            "Usage: ./ex4 DIRECTORY_NAME, where DIRECTORY_NAME can be a simple "
            "name or a longer file path.\n");
    return EXIT_FAILURE;
  }
  char* dirname;
  DIR* dirp;
  struct dirent* direntry;
  char* filepath;
  RO_FILE* fin;
  char readbuf[READBUFSIZE];
  size_t readlen;

  dirname = argv[1];
  dirp = opendir(dirname);
  if (dirp == NULL) {
    fprintf(stderr, "Error: Cannot open directory.\n");
    return EXIT_FAILURE;
  }

  direntry = readdir(dirp);
  while (direntry != NULL) {
    if (IsTxtFile(direntry->d_name)) {
      filepath = Concatenate(dirname, direntry->d_name);
      fin = ro_open(filepath);

      if (fin == NULL) {
        perror("fopen for read failed");
        free(filepath);
        return EXIT_FAILURE;
      }

      while ((readlen = ro_read(readbuf, 1, fin)) > 0) {
        if (readlen == -1) {
          perror("ro_read failed");
          ro_close(fin);
          free(filepath);
          return EXIT_FAILURE;
        }

        if (fwrite(readbuf, 1, readlen, stdout) < readlen) {
          perror("fwrite failed");
          ro_close(fin);
          free(filepath);
          return EXIT_FAILURE;
        }
      }
      ro_close(fin);
    }
    free(filepath);
    direntry = readdir(dirp);
  }
  closedir(dirp);

  return EXIT_SUCCESS;
}

/*** HELPER FUNCTION DEFINITIONS *******************************************/

bool IsTxtFile(char* filename) {
  char* dot = strrchr(filename, '.');
  return dot && !strcmp(dot, ".txt");
}

char* Concatenate(char* dirname, char* filename) {
  bool has_trailing_slash = dirname[strlen(dirname) - 1] == '/';
  // Concatenate directory and file name.
  size_t dlen = strlen(dirname);
  size_t flen = strlen(filename);
  // Malloc space for full path name:
  // dlen + strlen("/") + flen + strlen('\0') = dlen + flen + 2
  int size_to_malloc = has_trailing_slash ? dlen + flen + 1 : dlen + flen + 2;
  char* fullpath = (char*)malloc(sizeof(char) * (size_to_malloc));
  if (fullpath == NULL) {
    fprintf(stderr, "Error on malloc.\n");
    exit(EXIT_FAILURE);
  }
  if (has_trailing_slash) {
    snprintf(fullpath, size_to_malloc, "%s%s", dirname, filename);
  } else {
    snprintf(fullpath, size_to_malloc, "%s/%s", dirname, filename);
  }
  return fullpath;
}
