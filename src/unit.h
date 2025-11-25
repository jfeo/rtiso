#ifndef __RTISO_UNIT_H__
#define __RTISO_UNIT_H__

#include "action.h"
#include "entity.h"
#include "render_object.h"

struct unit {
  struct action *action;
  struct entity *entity;
};

struct unit *unit_create(struct render_object *robj, struct phys_radial *phys);

void unit_update(struct unit *unit);

#endif /* ifndef __RTISO_UNIT_H__ */
