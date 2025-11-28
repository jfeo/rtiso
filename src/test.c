#include "test.h"

#include "coord_test.c"
#include "unit_test.c"

int main() {
  int failed = 0;
  failed |= coord_test_suite();
  failed |= unit_test_suite();
  return failed;
}
