# Bug 1

## A) How is your program acting differently than you expect it to?
- WriteIndex does not produce correct index file. The magic number is correct. doctable_size and index_size are incorrect. The bucket_record of both doctable and index table is correct. There are also many repetitions of the same hex numbers inside the indexfile

## B) Brainstorm a few possible causes of the bug
- Maybe I didn't return the amount written correctly in on of the writing helper functions
- Maybe I wrote to the file incorrectly, used fwrite() incorrectly.
- Maybe I seeked to the wrong place in the index file when writing, gave wrong offset to fseek().
- Maybe I forgot to update the i or move the linkedlist to the next element in the for loops, so I got lots of repetitions in my index file.

## C) How you fixed the bug and why the fix was necessary
- The bug was caused by wrong use of fwrite() at line 430 when trying to write the file name into the index file. fwrite requires a pointer to an array of element to be written, and in this case it should be an array of characters. However, I passed in a pointer to a char* veriable, so the bytes that got written in are the addresses of the strings. To fix this bug, I just needed to pass in the original char* variable.

# Bug 2

## A) How is your program acting differently than you expect it to?
- Test_Suite failed for query processor on line 57 and 83. The rank for a single-index multi-word query is incorrect, and the result vector size for a multi-index single-word query is incorrect.

## B) Brainstorm a few possible causes of the bug
- 
- 
- 

## C) How you fixed the bug and why the fix was necessary
- 


# Bug 3

## A) How is your program acting differently than you expect it to?
- 

## B) Brainstorm a few possible causes of the bug
- 
- 
- 

## C) How you fixed the bug and why the fix was necessary
- 
