#ifndef __RTISO_UNIT_H__
#define __RTISO_UNIT_H__

#include "action.h"
#include "entity.h"

struct unit {
  struct action *action;
  struct entity *entity;
};

struct unit *unit_create(struct texture *tex, struct phys_radial *obj);

void unit_update(struct unit *unit);

#endif /* ifndef __RTISO_UNIT_H__ */
