#include <stddef.h>

#include "phys.h"
#include "test.h"
#include "unit.h"

static void test_unit_update_action_move() {
  struct phys_radial *r = phys_radial_create((struct coord_real){0});
  struct unit *u = unit_create(NULL, r);
  u->action.type = ACTION_MOVE;
  u->action.data.move.to.ne = 10.0;
  u->action.data.move.to.nw = 10.0;

  units_update(10.0);
  u->entity->phys->vel.ne = 5.0f;
  u->entity->phys->vel.nw = 5.0f;
  u->entity->phys->vel.up = 0.0f;

  free(u);
  free(r);
}

static int unit_test_suite() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_unit_update_action_move),
  };

  return cmocka_run_group_tests_name(__FILE_NAME__, tests, NULL, NULL);
}
