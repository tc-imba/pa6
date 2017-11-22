#include <sys/types.h>

extern char* memory;
extern size_t memory_size;

void setup_memory(size_t size);
void teardown_memory();
