#ifndef __RTISO_UNIT_H__
#define __RTISO_UNIT_H__

#include "action.h"
#include "array.h"
#include "entity.h"
#include "render_object.h"

ARRAY_DECLARE(struct unit *, unit)

struct unit {
  struct action action;
  struct entity *entity;
};

void units_init();
void units_update(float tdiff_ms);
struct unit *unit_create(struct render_object *robj, struct phys_radial *phys);

#endif /* ifndef __RTISO_UNIT_H__ */
