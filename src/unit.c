#include "unit.h"
#include "render_object.h"

#include <stdlib.h>

struct unit *unit_create(struct render_object *robj, struct phys_radial *phys) {
  struct unit *unit = (struct unit *)malloc(sizeof(struct unit));
  unit->entity = (struct entity *)malloc(sizeof(struct entity));
  unit->entity->render_obj = robj;
  unit->entity->phys = phys;
  unit->action = NULL;
  return unit;
}
