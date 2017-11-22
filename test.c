#include "cutest/CuTest.h"
#include "mem.h"
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int get_offset(void* p) {
	return (int)p - (int)memory;
}
int firstword(void* p) {
	return *((int*)p);
}

void TestSimple(CuTest *tc) {
	setup_memory(1024); // setup memory at 1kb

	int* i = my_malloc(sizeof(int) * 2);

	CuAssertIntEquals(tc, get_offset(i), 4);
	CuAssertIntEquals(tc, firstword(i), 9);
	CuAssertIntEquals(tc, free_space(), 1012);

	my_free(i);

	CuAssertIntEquals(tc, firstword(i), 8);

	CuAssertIntEquals(tc, free_space(), 1024);

	teardown_memory(); // always use setup_memory at the beginning and teardown at the end
}



/*
 * Note that as you add new testing functions, you need to add them to the list
 * below using SUITE_ADD_TEST. Otherwise they will not run.
 */
CuSuite* StrUtilGetSuite() {
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, TestSimple);
  return suite;
}
