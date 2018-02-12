#define _GNU_SOURCE
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

#include "malloc.h"

__attribute__((__visibility__("default")))
void free(void *ptr)
{
  if (!ptr)
    return;

  struct metadata *free = ptr;

  // Just change the value of the block's metadata in order to let malloc use it

  free--;
  free->state = FREE;
}
