#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

size_t HEAP_SIZE = 1028; // in bytes (e.g. 1kb of memory)
int WORD_SIZE = sizeof(int);

void* heap_ptr = NULL;
void* current_free_list = NULL;

void setup_heap() {
	// The mmap system call asks the operating system to please put HEAP_SIZE
	// memory into this process's address space. The resulting addresses
	// start at the very beginning of heap space (more or less). This is the
	// lowest-level interface to access memory without writing an operating
	// system yourself. It returns memory in fairly large chunks (usually
	// at least 1kb gets mapped at a time).
	heap_ptr = mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);

	// Set the first word on the heap to hold the total size available. See above for
	// a description of *(int*)ptr
	*(int*)heap_ptr = HEAP_SIZE - WORD_SIZE;
	*(void**)(heap_ptr+WORD_SIZE) = NULL;
	current_free_list = heap_ptr;
}
void teardown_heap(){
  munmap(heap_ptr, HEAP_SIZE);
  heap_ptr = NULL;
  current_free_list = NULL;
}

void *my_malloc(size_t size){

	return NULL;
}

int my_free(void *ptr){
	return 0;
}

void consolidate(){
	void *ptr = heap_ptr;
    void *first_free_ptr = NULL;
    void *prev_free_ptr = NULL;
    while(1) {
        int end = (ptr >= heap_ptr + HEAP_SIZE);
        int size = 0;
        int mod = 0;
        if (!end) {
            size = *(int *) ptr;
            mod = size % 4;
            if (mod) {
                size -= mod;
            } else {
                if (!first_free_ptr) {
                    first_free_ptr = ptr;
                }
            }
        }
        if (end || mod) {
            if (first_free_ptr) {
                *(int *) first_free_ptr = (int)(ptr - first_free_ptr) - WORD_SIZE;
                if (prev_free_ptr) {
                    *(void **)(prev_free_ptr + WORD_SIZE) = first_free_ptr;
                } else {
                    current_free_list = ptr;
                }
                prev_free_ptr = ptr;
                first_free_ptr = NULL;
            }
        }
        if (end) break;
        ptr += size + WORD_SIZE;
    }
    if (prev_free_ptr) {
        *(void **)(prev_free_ptr + WORD_SIZE) = NULL;
    }
}

void print_heap(){

}

int free_space(){
    void *ptr = current_free_list;
    int size = 0;
    while (ptr) {
        size += *(int *)ptr;
        ptr = *(void **)(ptr + WORD_SIZE);
    }
    return size;
}

int live_data(){
    void *ptr = heap_ptr;
    int size = 0;
    while (ptr < heap_ptr + HEAP_SIZE) {
        int _size = *(int *) ptr;
        int mod = size % 4;
        if (mod) {
            _size -= mod;
            size += _size;
        }
        ptr += _size + WORD_SIZE;
    }
    return size;
}
