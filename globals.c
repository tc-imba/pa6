#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "globals.h"

#define PAGE_SIZE (sysconf(_SC_PAGESIZE))
#define MEM_SIZE PAGE_SIZE
char *memory;
size_t memory_size;

void setup_memory(size_t size)  {
  memory = mmap(NULL, MEM_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  memory_size = size;
}

void teardown_memory() {
  munmap(memory, MEM_SIZE);
  memory = NULL;
}

