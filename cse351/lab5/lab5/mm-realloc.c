/*
 * CSE 351 Lab 5 (Dynamic Storage Allocator)
 * Extra credit: Implementing mm_realloc
 *
 * Name(s): Sebastian Liu
 * NetID(s): ll57
 *
 * NOTES:
 *  - This requires mm_malloc and mm_free to be working correctly, so don't
 *    start on this until you finish mm.c.
 *  - Only the traces that start with 'realloc' actually call this function,
 *    so make sure you actually test your code on these traces!
 *  - This file does not need to be submitted if you did not attempt it.
 */
#include "mm.c"

/*
 * EXTRA CREDIT:
 * Change the size of the memory block pointed to by ptr to size bytes while
 * preserving the existing data in range. If a new block is allocated during
 * this process, make sure to free the old block.
 *  - if ptr is NULL, equivalent to malloc(size)
 *  - if size is 0, equivalent to free(size)
 */
void *mm_realloc(void *ptr, size_t size)
{
  block_info *new_block = NULL;
  // TODO: Implement mm_realloc.
  // You will want to replace this return statement...
  if (ptr == NULL)
  {
    return mm_malloc(size);
  }
  else if (size == 0)
  {
    mm_free(ptr);
    return NULL;
  }
  else
  {
    block_info *old_block = (block_info *)UNSCALED_POINTER_SUB(ptr, WORD_SIZE);
    size_t old_block_size = SIZE(old_block->size_and_tags);
    size_t new_block_size = WORD_SIZE + size;
    if (old_block_size == new_block_size)
    {
      return ptr;
    }
    else if (old_block_size > new_block_size)
    {
      block_info *split_block = (block_info *)UNSCALED_POINTER_ADD(old_block, new_block_size);
      split_block->size_and_tags = (old_block_size - new_block_size) | TAG_PRECEDING_USED;

      // Initialize the footer of the new block.
      ((block_info *)UNSCALED_POINTER_ADD(split_block, old_block_size - new_block_size - WORD_SIZE))->size_and_tags =
          (old_block_size - new_block_size) | TAG_PRECEDING_USED;

      // Add the new block to the free list and immediately coalesce newly
      // allocated memory space.
      insert_free_block(split_block);
      coalesce_free_block(split_block);
      size_t preceding_block_use_tag = (old_block->size_and_tags) & TAG_PRECEDING_USED;
      old_block->size_and_tags = new_block_size | preceding_block_use_tag | TAG_USED;

      return (void *)UNSCALED_POINTER_ADD(old_block, WORD_SIZE);
    }
    else
    {
      new_block = mm_malloc(size);
      for (int i = 0; i < (old_block_size - WORD_SIZE); i += WORD_SIZE)
      {
        *(long *)UNSCALED_POINTER_ADD(new_block, WORD_SIZE + i) = *(long *)UNSCALED_POINTER_ADD(old_block, WORD_SIZE + i);
      }
      mm_free(ptr);
    }
  }

  return (void *)UNSCALED_POINTER_ADD(new_block, WORD_SIZE);
}
