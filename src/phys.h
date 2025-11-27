#ifndef __RTISO_PHYS_H__
#define __RTISO_PHYS_H__

#include "array.h"
#include "coord.h"
#include "map.h"

#define RTISO_GRAV_CONST 1.0

struct phys_radial {
  struct coord_real pos;
  struct coord_real vel;
  double mass;
  double radius;
  int flying;
};

ARRAY_DECLARE(struct phys_radial *, phys_radial)

extern array_phys_radial radials;

void phys_init();
struct phys_radial *phys_radial_create(struct coord_real pos);
void phys_radial_update(double tdiff_ms, struct phys_radial *obj);
int phys_collision(struct phys_radial *a, struct phys_radial *b);
void phys_update(double tdiff_ms);

#endif /* ifndef __RTISO_PHYS_H__ */
