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
- Failing Test on test_memindex.cc:89. NULL is returned from MemIndex_Search but it supposed to be non-empty linked list.

## B) Brainstorm a few possible causes of the bug
- Maybe the word posting wasn't added to the Index correctly in MemIndex_AddPostingList. As a result NULL gets returned in MemeIndexSearch at line 179, when we can't find the key inside the the index.
- Maybe he SearchResults was not added to the ret_list correctly. As a result, in MemIndex_Search line 240, all the Linked List Nodes were removed because we couldn't find a match in postings.
- Maybe I forgot to call the next on iterators when going throught the word postings or the ret_list, so the query cant find a match and returned NULL.

## C) How you fixed the bug and why the fix was necessary
- I used gdb to step through the for loop on line 234, and foud out I'm getting really big numbers for doc_ids I got from the word postings. It turns out that i passed in the wrong output parameter when using LLIterator_Get. The function needed a pointer to a pointer, and i passed in a pointer to SearchResults. So the weird doc_ids i got from gdb were memory addresses for the word postings, they made the loop to go through the ret_list and deleted all the nodes, and that's why MemIndex_Search returned NULL in the test.


# Bug 3

## A) How is your program acting differently than you expect it to?
- searchshell does not display the results in the subdirectories, and gives a segfault when giveing ctrl+d as a entry.

## B) Brainstorm a few possible causes of the bug
- 
- 
- 

## C) How you fixed the bug and why the fix was necessary
- 
