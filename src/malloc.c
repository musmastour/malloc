#define _GNU_SOURCE
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

#include "malloc.h"

static struct metadata *g_first_block;

void *cast(void *ptr, size_t size)
{
  char *tmp = ptr;
  return tmp + size;
}

size_t nb_page_to_allocate(size_t size)
{
  return 1 + ((size + sizeof(struct metadata)) / PAGESIZE);
}

struct metadata *init_first_block(size_t size)
{
  size_t nb_pages = nb_page_to_allocate(size);
  void *ptr = mmap(NULL, 
                  (nb_pages * PAGESIZE), 
                  PROT_READ | PROT_WRITE, 
                  MAP_PRIVATE | MAP_ANONYMOUS, 
                  -1, 
                  0);

  if (ptr == MAP_FAILED)
    return NULL;

  g_first_block = ptr;
  g_first_block->state = FREE;
  g_first_block->size = nb_pages * PAGESIZE - sizeof(struct metadata);

  return g_first_block;
}

struct metadata *extend_memory(size_t size, struct metadata *block)
{
  while (block->next)
    block = block->next;

  struct metadata *last = block;
  size_t nb_pages = nb_page_to_allocate(size);
  void *ptr = mmap(NULL, 
                  (nb_pages * PAGESIZE), 
                  PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS, 
                  -1, 
                  0);

  if (ptr == MAP_FAILED)
    return NULL;

  last->next = ptr;
  last->next->state = FREE;
  last->next->next = NULL;
  last->next->size = nb_pages * PAGESIZE - sizeof(struct metadata);

  return g_first_block;
}

void create_next_block(struct metadata *block, size_t size)
{
  struct metadata *new = block;
  size_t tmp_size = block->size;
  
  block->state = USED;
  block->size = size;
  
  new++;
  new = cast(new, size);
  new->state = FREE;
  new->size = tmp_size - size - sizeof(struct metadata);
  new->next = block->next;
  
  block->next = new;

}

struct metadata *search_free_memory(size_t size)
{
  struct metadata *block = g_first_block;

  if (size == 0)
    return NULL;

  do {

    if (block->state == FREE && size + sizeof(struct metadata) < block->size)
    {
      create_next_block(block, size);
      return block;
    }

    else if (block->state == FREE && size <= block->size)
    {
      block->state = USED;
      return block;
    }

    else
    {
      if (!block->next)
        block = extend_memory(size, block);

      if (!block)
        return NULL;

      block = block->next;
    }

  }while (block != NULL);

  return NULL;
}

size_t change_size(size_t size)
{
  if (size % sizeof(size_t) == 0)
    return size;

  else
  {
    size_t rest = size % sizeof(size_t);
    size += sizeof(size_t) - rest;
  }

  return size;
}

__attribute__((__visibility__("default")))
void *malloc(size_t size) 
{
  size = change_size(size);

  if(!g_first_block)
    g_first_block = init_first_block(size);

  struct metadata *current_block = search_free_memory(size);

  if (!current_block)
    return NULL;

  current_block++;

  return current_block;
}
