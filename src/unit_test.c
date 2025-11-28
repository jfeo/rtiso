#include <stddef.h>

#include "constants.h"
#include "phys.h"
#include "test.h"
#include "unit.h"

#define EPSILON 0E-5

static void test_unit_update_action_move() {
  struct phys_radial *r = phys_radial_create((struct coord_real){0});
  struct unit *u = unit_create(NULL, r);
  u->action.type = ACTION_MOVE;
  u->action.data.move.to.ne = 10.0;
  u->action.data.move.to.nw = 10.0;

  units_update(10.0);

  float expected = 5.0 * 10.0 / (SQRT_2 * 10.0f);
  assert_float_equal(u->entity->phys->vel.ne, expected, EPSILON);
  assert_float_equal(u->entity->phys->vel.nw, expected, EPSILON);
  assert_float_equal(u->entity->phys->vel.up, 0.0f, EPSILON);

  free(u);
  free(r);
}

static int unit_test_suite() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_unit_update_action_move),
  };

  return cmocka_run_group_tests_name(__FILE_NAME__, tests, NULL, NULL);
}
