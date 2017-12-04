#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

size_t HEAP_SIZE = 1028; // in bytes (e.g. 1kb of memory)
int WORD_SIZE = sizeof(int);

void *heap_ptr = NULL;
void *current_free_list = NULL;

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
    *(int *) heap_ptr = HEAP_SIZE - WORD_SIZE;
    *(void **) (heap_ptr + WORD_SIZE) = NULL;
    current_free_list = heap_ptr;
}
void teardown_heap() {
    munmap(heap_ptr, HEAP_SIZE);
    heap_ptr = NULL;
    current_free_list = NULL;
}

void *my_malloc(size_t size) {
    if (size == 0 || size >= HEAP_SIZE) return NULL;

    // round up size
    size_t mod = size % WORD_SIZE;
    if (mod) size += WORD_SIZE - mod;

    // find a free space and malloc
    void *free_ptr = current_free_list;
    void *prev_free_ptr = NULL;
    while (free_ptr) {
        int *free_ptr_int = (int *) free_ptr;
        if (*free_ptr_int < size) {
            prev_free_ptr = free_ptr;
            free_ptr = *(void **) (free_ptr + WORD_SIZE);
            continue;
        }
        void *next_free_ptr;
        if (*free_ptr_int <= size + WORD_SIZE) {
            next_free_ptr = *(void **) (free_ptr + WORD_SIZE);
            ++(*free_ptr_int);
        } else {
            next_free_ptr = free_ptr + size + WORD_SIZE;
            *(int *) next_free_ptr = *free_ptr_int - (int) size - WORD_SIZE;
            *(void **) (next_free_ptr + WORD_SIZE) = *(void **) (free_ptr + WORD_SIZE);
            *free_ptr_int = (int) size + 1;
        }
        if (prev_free_ptr) {
            *(void **) (prev_free_ptr + WORD_SIZE) = next_free_ptr;
        } else {
            current_free_list = next_free_ptr;
        }
        *(void **) (free_ptr + WORD_SIZE) = NULL; // easier for debug
        return free_ptr + WORD_SIZE;
    }

    // free space not found
    return NULL;
}

int my_free(void *ptr) {
    ptr -= WORD_SIZE;
    int size = *(int *) ptr;
    if (size < WORD_SIZE || size % WORD_SIZE != 1) return 0;
    --(*(int *) ptr);

    *(void **) (ptr + WORD_SIZE) = current_free_list;
    current_free_list = ptr;

    return 1;
}

void consolidate() {

}

void print_heap() {
    printf("current: [%u]\n", (size_t) current_free_list - (size_t) heap_ptr);
    unsigned int *ptr = heap_ptr;
    for (int i = 0; i < HEAP_SIZE; i += 4) {
        if (*ptr >= (size_t) heap_ptr) {
            printf("[%u] ", *ptr - (size_t) heap_ptr);
        } else {
            printf("%u ", *ptr);
        }
        ptr += 1;
    }
    printf("\n");
}

int free_space() {
    return 0;
}

int live_data() {
    return 0;
}
