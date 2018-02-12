#define _GNU_SOURCE
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

#include "malloc.h"

__attribute__((__visibility__("default")))
void *calloc(size_t number, size_t size)
{
  size_t alloc = number * size;

  if (alloc / size != number)
    return NULL;

  void *ptr = malloc(alloc);

  ptr = memset(ptr, 0, alloc);

  return ptr;
}
