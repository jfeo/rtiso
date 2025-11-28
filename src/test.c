#include "test.h"

#include "coord_test.c"
#include "unit_test.c"
#include <stdio.h>

int main() {
  int failed = 0;
  failed += coord_test_suite();
  failed += unit_test_suite();
  printf("[==========] Total result:\n");
  if (failed) {
    printf("\n%d TOTAL FAILED TEST(s).\n", failed);
  } else {
    printf("[  PASSED  ]\n");
  }
  return failed;
}
