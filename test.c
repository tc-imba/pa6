#include "cutest/CuTest.h"
#include "alloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
    
/*
 * Your tests won't look like this; they will use helpers like makeNode and
 * call the actual bst_ functions. This is here to give you an example of
 * creating space for a BSTNode with malloc and using a testing function to
 * check the string.
 *
 */
void TestMyMalloc(CuTest *tc) {
setup_heap();
void *p1 = my_malloc(0);
CuAssertPtrEquals(tc, NULL, p1);
teardown_heap();
}


void TestMyFree(CuTest * tc){
setup_heap();
int result = my_free(current_free_list);
CuAssertIntEquals(tc, 0,result);
teardown_heap();
}

/*
 * Note that as you add new testing functions, you need to add them to the list
 * below using SUITE_ADD_TEST. Otherwise they will not run.
 */
CuSuite* StrUtilGetSuite() {
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, TestMyMalloc);
  SUITE_ADD_TEST(suite, TestMyFree);
  return suite;
}
