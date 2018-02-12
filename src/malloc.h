#ifndef MALLOC_H
#define MALLOC_H

#include <stdio.h>

// Blockstate

#define USED 1
#define FREE 0

#define PAGESIZE sysconf(_SC_PAGESIZE)

struct metadata
{
  long state;
  size_t size;
  
  struct metadata *next;
};


size_t nb_page_to_allocate(size_t size);
void *cast(void *ptr, size_t size);
struct metadata *init_first_block(size_t size);
void create_next_block(struct metadata *block, size_t size);
struct metadata * extend_memory(size_t size, struct metadata *block);
size_t check_memory_left(size_t size);
size_t change_size(size_t size);
long check_free_size(size_t size);
void *malloc(size_t size);
void free(void *ptr);

//static struct metadata *g_first_block; // Here malloc was never called yet

#endif /* !MALLOC_H */
