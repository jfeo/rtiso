#include "unit.h"
#include "render_object.h"

#include <stdlib.h>

ARRAY_DEFINE(struct unit *, unit)

array_unit units;

struct unit *unit_create(struct render_object *robj, struct phys_radial *phys) {
  struct unit *unit = (struct unit *)malloc(sizeof(struct unit));
  unit->entity = (struct entity *)malloc(sizeof(struct entity));
  unit->entity->render_obj = robj;
  unit->entity->phys = phys;
  array_unit_add(&units, unit);
  return unit;
}

void unit_update_action_move(struct unit *u) {
  struct action_move *move = &u->action.data.move;
  struct coord_real diff = coord_real_sub(move->to, u->entity->phys->pos);
  struct coord_real dir = coord_real_normalize(diff);
  dir.ne *= 5;
  dir.nw *= 5;
  dir.up *= 5;
  u->entity->phys->vel = dir;
}

void unit_update_action(struct unit *u) {
  switch (u->action.type) {
  case ACTION_MOVE:
    unit_update_action_move(u);
    break;
  case ACTION_NONE:
    break;
  }
}

void unit_update(struct unit *u) { unit_update_action(u); }

void units_init() { units = array_unit_init(); }

void units_update(float tdiff_ms) {
  for (int i = 0; i < units.count; i++) {
    unit_update(units.elems[i]);
  }
}
