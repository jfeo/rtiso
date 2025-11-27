#include "phys.h"
#include "array.h"

#include <stdio.h>
#include <stdlib.h>

ARRAY_DEFINE(struct phys_radial *, phys_radial)

array_phys_radial radials;

void phys_init() { radials = array_phys_radial_init(); };

void phys_update(double tdiff_ms) {
  for (int i = 0; i < radials.count; i++) {
    printf("radial[%d]:\n", i);
    phys_radial_update(tdiff_ms, radials.elems[i]);
  }
}

struct phys_radial *phys_radial_create(struct coord_real pos) {
  struct phys_radial *obj =
      (struct phys_radial *)malloc(sizeof(struct phys_radial));
  obj->pos = pos;
  obj->radius = 0.5f;
  obj->mass = 1.0f;
  obj->flying = 0;

  obj->vel.ne = 0.0f;
  obj->vel.nw = 0.0f;
  obj->vel.up = 0.0f;

  array_phys_radial_add(&radials, obj);

  return obj;
}

void phys_radial_update(double tdiff_ms, struct phys_radial *obj) {
  obj->pos.nw += obj->vel.nw * tdiff_ms / 1000.0;
  obj->pos.ne += obj->vel.ne * tdiff_ms / 1000.0;
  obj->pos.up += obj->vel.up * tdiff_ms / 1000.0;

  printf("  pos = (nw: %f, ne: %f, up: %f)\n", obj->pos.nw, obj->pos.ne,
         obj->pos.up);
  printf("  vel = (nw: %f, ne: %f, up: %f)\n", obj->vel.nw, obj->vel.ne,
         obj->vel.up);
  ;

  if (obj->pos.up < 0.0f) {
    obj->vel.up = 0.0f;
    obj->flying = 0;
  }

  if (obj->flying) {
    obj->vel.up -= RTISO_GRAV_CONST;
  }
}

int phys_collision(struct phys_radial *a, struct phys_radial *b) {
  return coord_real_dist(a->pos, b->pos) < a->radius + b->radius;
}
