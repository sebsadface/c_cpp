#include "ro_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

static const int RO_FILE_BUF_LEN = 512;

struct ro_file_st {
  int fd;

  char* buf;
  off_t buf_pos;

  int buf_index;

  int buf_end;
};

static size_t flush_buffer(RO_FILE* file, char* out, int amount);

static ssize_t fill_buffer(RO_FILE* file);

RO_FILE* ro_open(char* filename) {
  // Allocate a new RO_FILE
  RO_FILE* result = (RO_FILE*)malloc(sizeof(RO_FILE));
  if (result == NULL) {
    return NULL;
  }
  // Get the file descriptor for the file
  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    free(result);
    return NULL;
  }
  result->fd = fd;
  // Allocate the internal buffer
  result->buf = (char*)malloc(sizeof(char) * RO_FILE_BUF_LEN);
  if (result->buf == NULL) {
    free(result->buf);
    free(result);
    close(fd);
    return NULL;
  }
  // Initialize the other fields (no reading done yet)
  result->buf_pos = 0;
  result->buf_index = 0;
  result->buf_end = 0;
  return result;
}

ssize_t ro_read(char* ptr, size_t len, RO_FILE* file) {
  // 1. If we have bytes in our internal buffer, flush as many as we can to
  //    'ptr'.
  size_t num_copied_out = flush_buffer(file, ptr, len);

  while (num_copied_out != len) {
    // 2. If we haven't provided the bytes requested, repopulate our buffer
    //    with bytes from the file.
    ssize_t num_filled = fill_buffer(file);
    if (num_filled == 0) {
      // reached EOF; no more bytes to copy
      break;
    } else if (num_filled < 0) {
      // pass error back to client
      return num_filled;
    }

    // 3. Copy filled bytes into 'ptr'.
    num_copied_out +=
        flush_buffer(file, ptr + num_copied_out, len - num_copied_out);

    // 4. Repeat steps 2-3 until request is fulfilled.
  }

  return num_copied_out;
}

off_t ro_tell(RO_FILE* file) {
  if (file == NULL) {
    return -1;
  }
  return file->buf_pos + file->buf_index;
}

int ro_seek(RO_FILE* file, off_t offset, int whence) {
  if (file == NULL) {
    return 1;
  }
  // Seek to specified offset from specified whence
  off_t res = lseek(file->fd, offset, whence);
  if (res == -1) {
    return 1;
  }
  // Update the fields
  // Note: this is a naive implementation as we stated that you didn't have to
  // determine if the resulting offset is a position in our buffer.
  file->buf_pos = res;
  file->buf_index = 0;
  file->buf_end = 0;
  return 0;
}

int ro_close(RO_FILE* file) {
  if (file == NULL) {
    return -1;
  }
  // Clean up resources, returns non-zero on error
  int res = close(file->fd);
  free(file->buf);
  free(file);
  return res;
}

/*** STATIC HELPER FUNCTION DEFINITIONS *************************************/

size_t flush_buffer(RO_FILE* file, char* out, int amount) {
  // Flush bytes from buffer to "out" one byte at a time with
  // bytes to flush = min(amount, bytes available in buffer),
  // accumulating total number of bytes flushed
  int bytes_flushed;
  for (bytes_flushed = 0; bytes_flushed < (file->buf_end - file->buf_index) &&
                          bytes_flushed < amount;
       bytes_flushed++) {
    out[bytes_flushed] = file->buf[file->buf_index + bytes_flushed];
  }
  // Advance index into the buffer by number of bytes flushed
  file->buf_index += bytes_flushed;
  return bytes_flushed;
}

ssize_t fill_buffer(RO_FILE* file) {
  // Point into the file where the buffer left off
  file->buf_pos += file->buf_index;
  // Reset buffer index and len
  file->buf_index = 0;
  file->buf_end = 0;
  // Accumulate number of bytes buffer
  int bytes_buffered = 0;
  // POSIX read loop that attempts to buffer RO_FILE_BUF_LEN bytes
  while (bytes_buffered < RO_FILE_BUF_LEN) {
    ssize_t res = read(file->fd, (file->buf) + bytes_buffered,
                       RO_FILE_BUF_LEN - bytes_buffered);
    // Reached EOF
    if (res == 0) {
      break;
      // Reached error, check for error code
    } else if (res < 0) {
      if (errno == EINTR || errno == EAGAIN) {
        continue;
      }
      // Return -1 on POSIX error
      return res;
    }
    bytes_buffered += res;
  }
  // Update buffer status
  file->buf_end = bytes_buffered;
  return bytes_buffered;
}