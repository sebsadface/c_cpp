# Bug 1

## A) How is your program acting differently than you expect it to?

- Segfault when running Test_FileParser.ParseIntoWordPositionsTable.
  gdb says I got a double free or corruption (!prev) error when the function
  ParseIntoWordPositionsTable is freeing the memory for file_contents after
  inserting all the contents into the hash table.

## B) Brainstorm a few possible causes of the bug
- Maybe I accidentally freed the memory for file_contents in my InsertContent or AddWordPosition function.
- Maybe I overflowed the memory (corrupted the memory) for file_contents and overwritten the heap boundary tags when stepping through file_contents and checking for boundary characters.
- Maybe I used the same block of memory on the heap in two different places.

## C) How you fixed the bug and why the fix was necessary
- I allocated new memory for the word we are insterting into the hash table. Before I was inserting word_start straight into the hash table, which points to the block of memory allocated for file_contents. This caused problems because when I free the memory for file_contents, the words inserted into the hash table are lost, and when I free the memory for hash_table, the memory for the words got freed again.


# Bug 2

## A) How is your program acting differently than you expect it to?
- 

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
