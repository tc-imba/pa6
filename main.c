//
// Created by liu on 17-12-4.
//

#include "alloc.h"
#include <stdio.h>

int main() {
    setup_heap();

    print_heap();

    void *p1 = my_malloc(1);
    print_heap();

    void *p2 = my_malloc(18);
    print_heap();

    my_free(p2);
    print_heap();

    void *p3 = my_malloc(8);
    print_heap();

    void *p4 = my_malloc(12);
    print_heap();

    void *p5 = my_malloc(5);
    print_heap();

    my_free(p1);
    print_heap();

    my_free(p4);
    print_heap();

    my_free(p3);
    print_heap();

    printf("free space: %d\n", free_space());
    printf("live data: %d\n\n", live_data());

    consolidate();
    print_heap();

    printf("free space: %d\n", free_space());
    printf("live data: %d\n\n", live_data());

    teardown_heap();
}