# Bug 1

## A) How is your program acting differently than you expect it to?
  
- The stored element value after calling HashTable_Insert doesn't match the original inserted value. The output pointer oldkeyvalue doesn't match the previouse inserted value that has they same key.

## B) Brainstorm a few possible causes of the bug

- I think maybe because the newkeyvalue passed into the function is not located in the heap, so the value pushed into the hashtable gets lost when the caller is deallocated from the stack.
- Maybe the variable type of oldkeyvalue doesn't match the variable type returned by LLIterator_Get. So when we try to use oldkeyvalue, we get an unexpected value.
- Maybe I made a careless mistake when copying values from the linked list node into the output pointer, or passed a incorrect argument into the LLIterator_Get function.

## C) How you fixed the bug and why the fix was necessary

- Allocated memory for newkeyvalue on the heap, so the new value wont lose after inserted into the hash table. Created a new HTkeyValue * variable to copy value from the linked list node's payload to the output varibale oldkeyvalue, and not change the output value when update the payload with the new value that has the same key. Passed a argument of type HTKeyValue ** to LLIterator_Get, to match the LLPayload_t *payload data type that LLIterator_Get accepts.

# Bug 2

## A) How is your program acting differently than you expect it to?

- Having a mysterious segfault when try to access the value of a HTKeyValue struct that I got from HTIterator_Get after filling up the hash table.

## B) Brainstorm a few possible causes of the bug

- Maybe I did not correctly insert the element when filling up the hash table. Maybe my HashTable_Insert function added non-pointer arguments to the value field of the HTKeyValue struct, so when try to use the value field, we are accessing parts of the memory that's not readable.
- Maybe the Iterator_Get function didn't output the correct pointer to the HTKeyValue struct. Maybe the HT iterator was not pointing at a valid linked list node.
- Maybe the HTIterator_Next function didn't correctly move to the next valid element in the hash table, maybe it move the iterator passed the end of a bucket.

## C) How you fixed the bug and why the fix was necessary
  
- It turns out to be a silly mistake.The bug occured because the payload of the linked list node wasn't properly copied into the output pointer. I switched the lefthand side and right hand side of the copy operations, and then everything worked.


# Bug 3

## A) How is your program acting differently than you expect it to?

- I get memory leaks from 11 calls to the function HashTable_insert.

## B) Brainstorm a few possible causes of the bug
  
- I think it's because i didn't free up the memory allocated for the newkeyvalue I used to insert the newkeyvalue into the hash table.
- Maybe the memory wasn't freed after removing the keyvalue from the hash table.
- Maybe the memory wasn't freed properly after freeing the whole hash table

## C) How you fixed the bug and why the fix was necessary

- I freed the memory allocated for the newkeyvalue after the key was found in the hash table. Also freed the memory for the payload when calling HTIterator_remove
