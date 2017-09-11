#ifndef __RTISO_PHYS_H__
#define __RTISO_PHYS_H__

#include "map.h"
#include "coord.h"

#define RTISO_GRAV_CONST 1.0

struct phys_radial {
	struct coord_real pos;
	struct coord_real vel;
	double mass;
	double radius;
	int flying;
};

struct phys_radial* phys_radial_create();

void phys_radial_update(struct map* map, struct phys_radial* obj);

int phys_collision(struct phys_radial* a, struct phys_radial* b);

#endif /* ifndef __RTISO_PHYS_H__ */
