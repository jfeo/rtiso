#include "coord.h"
#include "test.h"

static void test_coord_tile_from_camera_zero(void **state) {
  struct coord_camera cam = {0.0, 0.0};

  struct coord_tile tile = coord_tile_from_camera(cam, 0);

  assert_true(tile.ne == 0);
  assert_true(tile.nw == 0);
  assert_true(tile.up == 0);
}

static void test_coord_tile_from_real_zero(void **state) {
  struct coord_real real = {0.0f, 0.0f, 0.0f};

  struct coord_tile tile = coord_tile_from_real(real);

  assert_int_equal(tile.ne, 0);
  assert_int_equal(tile.nw, 0);
  assert_int_equal(tile.up, 0);
}

static void test_coord_tile_from_real_floors(void **state) {
  struct coord_real real = {1.5f, 5.53f, 999.99f};

  struct coord_tile tile = coord_tile_from_real(real);

  assert_int_equal(tile.nw, 1);
  assert_int_equal(tile.up, 5);
  assert_int_equal(tile.ne, 999);
}

static int coord_test_suite() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_coord_tile_from_camera_zero),
      cmocka_unit_test(test_coord_tile_from_real_zero),
      cmocka_unit_test(test_coord_tile_from_real_floors),
  };

  return cmocka_run_group_tests_name(__FILE_NAME__, tests, NULL, NULL);
}
