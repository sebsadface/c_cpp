// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include <stdio.h>    // for snprintf
#include <stdlib.h>   // for EXIT_SUCCESS, NULL, malloc, free
#include <string.h>   // for strrchr, strcmp, strlen
#include <stdbool.h>  // for bool
#include <dirent.h>   // DIR

#include "ro_file.h"

#define READBUFSIZE 1024  // size of the read buffer
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
  // Check command line arguments
  if (argc != 2) {
    fprintf(stderr,
            "Usage: ./ex4 DIRECTORY_NAME, where DIRECTORY_NAME can be a simple "
            "name or a longer file path.\n");
    return EXIT_FAILURE;
  }

  DIR* dirp;  // a directory stream (used similarly to a file descriptor)
  struct dirent* direntry;    // describe a directory entry
  char* dirpath;              // the full path to the specified directory
  RO_FILE* fin;               // file descriptor
  char readbuf[READBUFSIZE];  // arbitrarily sized buffer
  size_t readlen;

  // Open the specified directory.
  dirp = opendir(argv[1]);
  if (dirp == NULL) {
    fprintf(stderr, "Error: Cannot open directory.\n");
    return EXIT_FAILURE;
  }

  // Get the next directory entry.
  direntry = readdir(dirp);

  // Go through all the directory entries
  while (direntry != NULL) {
    // Check if the current file is a .txt file
    if (IsTxtFile(direntry->d_name)) {
      // Get the full path to the .txt file
      dirpath = Concatenate(argv[1], direntry->d_name);

      // Opent the .txt file
      fin = ro_open(dirpath);
      if (fin == NULL) {
        perror("fopen for read failed");
        free(dirpath);
        return EXIT_FAILURE;
      }

      // Read through the .txt file and fill them into our read buffer
      while ((readlen = ro_read(readbuf, READBUFSIZE, fin)) > 0) {
        if (readlen == -1) {
          perror("ro_read failed");
          ro_close(fin);
          free(dirpath);
          return EXIT_FAILURE;
        }

        // Print the bytes in our read buffer to std out.
        if (fwrite(readbuf, 1, readlen, stdout) < readlen) {
          perror("fwrite failed");
          ro_close(fin);
          free(dirpath);
          return EXIT_FAILURE;
        }
      }

      // Clean up
      ro_close(fin);
      free(dirpath);
    }

    // Get the next directory entry.
    direntry = readdir(dirp);
  }

  // Clean up
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
