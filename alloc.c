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
        return free_ptr + WORD_SIZE;
    }

    // free space not found
    return NULL;
}

int my_free(void *ptr) {
    ptr -= WORD_SIZE;
    if (ptr < heap_ptr || ptr >= heap_ptr + HEAP_SIZE) return 0;
    int size = *(int *) ptr;
    if (size < WORD_SIZE || size % WORD_SIZE != 1) return 0;
    --(*(int *) ptr);

    *(void **) (ptr + WORD_SIZE) = current_free_list;
    current_free_list = ptr;

    return 1;
}

void consolidate() {
    void *ptr = heap_ptr;
    void *first_free_ptr = NULL;
    void *prev_free_ptr = NULL;
    while (1) {
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
                *(int *) first_free_ptr = (int) (ptr - first_free_ptr) - WORD_SIZE;
                if (prev_free_ptr) {
                    *(void **) (prev_free_ptr + WORD_SIZE) = first_free_ptr;
                } else {
                    current_free_list = first_free_ptr;
                }
                prev_free_ptr = first_free_ptr;
                first_free_ptr = NULL;
            }
        }
        if (end) break;
        ptr += size + WORD_SIZE;
    }
    if (prev_free_ptr) {
        *(void **) (prev_free_ptr + WORD_SIZE) = NULL;
    }
}

void print_heap() {
    printf("current_free_list: %u\n", (size_t) current_free_list - (size_t) heap_ptr);
    void *ptr = heap_ptr;
    while (ptr < heap_ptr + HEAP_SIZE) {
        int size = *(int *) ptr;
        int mod = size % 4;
        printf("addr: %d  \tlen: %d  \t", ptr - heap_ptr, *(int *) ptr);
        if (mod) {
            size -= mod;
        }
        else {
            void* _ptr = *(void **) (ptr + WORD_SIZE);
            if (_ptr) {
                printf("-> addr: %d", _ptr - heap_ptr);
            } else {
                printf("end");
            }
        }
        printf("\n");
        ptr += size + WORD_SIZE;
    }
    printf("\n");
}

int free_space() {
    void *ptr = current_free_list;
    int size = 0;
    while (ptr) {
        size += *(int *) ptr;
        ptr = *(void **) (ptr + WORD_SIZE);
    }
    return size;
}

int live_data() {
    void *ptr = heap_ptr;
    int size = 0;
    while (ptr < heap_ptr + HEAP_SIZE) {
        int _size = *(int *) ptr;
        int mod = _size % 4;
        if (mod) {
            _size -= mod;
            size += _size;
        }
        ptr += _size + WORD_SIZE;
    }
    return size;
}
