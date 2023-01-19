/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#ifndef HW1_HASHTABLE_PRIV_H_
#define HW1_HASHTABLE_PRIV_H_

#include <stdint.h>  // for uint32_t, etc.

#include "./LinkedList.h"
#include "./HashTable.h"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Internal structures and helper functions for our HashTable implementation.
//
// These would typically be located in HashTable.c; however, we have broken
// them out into a "private .h" so that our unittests can access them.  This
// allows our test code to peek inside the implementation to verify correctness.
//
// Customers should not include this file or assume anything based on
// its contents.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// The hash table implementation.
//
// A hash table is an array of buckets, where each bucket is a linked list
// of HTKeyValue structs.
typedef struct ht {
  int num_buckets;       // # of buckets in this HT?
  int num_elements;      // # of elements currently in this HT?
  LinkedList **buckets;  // the array of buckets
} HashTable;

// The hash table iterator.
typedef struct ht_it {
  HashTable *ht;          // the HT we're pointing into
  int bucket_idx;         // which bucket are we in?
  LLIterator *bucket_it;  // iterator for the bucket, or NULL
} HTIterator;

// This is the internal hash function we use to map from HTKey_t keys to a
// bucket number.
int HashKeyToBucketNum(HashTable *ht, HTKey_t key);

// Find and optionally remove a keyvalue from the hash table
//
// Arguments:
// - table: the hash table to search
// - key: the key for the wanted keyvalue pair
// - remove: whether to remove the keyvalue pair once found
// - keyvaluefound: a pointer to the keyvalue pair found using the given key
//
// Returns:
//
// - true if the keyvalue pair was found, false otherwise
bool FindKeyValue(HashTable *table, HTKey_t key, bool remove,
                  HTKeyValue_t **keyvaluefound);

// Copy a keyvalue from a source to a destination and optionally free the memory
// of a source.
//
// Arguments:
// - source: the keyvalue pair that will be copied from and opionally freed
// - destination: the destination that the keyvalue pair will be copied to
// - free_source: whether to free the memory of the souce keyvalue pair
void CopyAndFree(HTKeyValue_t *source, HTKeyValue_t *dest, bool free_source);

#endif  // HW1_HASHTABLE_PRIV_H_
