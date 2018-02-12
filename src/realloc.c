#define _GNU_SOURCE
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

#include "malloc.h"

__attribute__((__visibility__("default")))
void *realloc(void *ptr, size_t size)
{
  if (ptr == NULL)
    return malloc(size);

  struct metadata *origin = ptr;
  origin--;

  size = change_size(size);
  if (origin->size > size + sizeof(struct metadata))
  {
    struct metadata *new = origin;

    new++;
    new = cast(new, size);
    new->state = FREE;
    new->size = origin->size - size - sizeof(struct metadata);
    new->next = origin->next;

    origin->next = new;
  }

  else if (origin->size <= size + sizeof(struct metadata))
  {
    void *new = malloc(size);
    new = memcpy(new, ptr, size);
    free(ptr);

    return new;
  }

  return ptr;
}
