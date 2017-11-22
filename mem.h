#include <sys/types.h>
#include <stddef.h>

void* my_malloc(size_t size);
void my_free(void *ptr);
void print_memory();
void consolidate();
size_t free_space();
