#include "phys.h"

#include <stdlib.h>

struct phys_radial* phys_radial_create(struct coord_real pos) {
	struct phys_radial* obj = (struct phys_radial*)malloc(sizeof(struct phys_radial));
	obj->pos = pos;
	obj->radius = 0.5f;
	obj->mass = 1.0f;
	obj->flying = 1;
	return obj;
}

void phys_radial_update(struct map* map, struct phys_radial* obj) {
	obj->pos.nw += obj->vel.nw;
	obj->pos.ne += obj->vel.ne;
	obj->pos.up += obj->vel.up;

	if (obj->pos.up < 0.0f) {
		obj->vel.up = 0.0f;
		obj->flying = 0;
	}

	if (obj->flying) {
		obj->vel.up -= RTISO_GRAV_CONST;
	}
}

int phys_collision(struct phys_radial* a, struct phys_radial* b) {
	return coord_real_dist(a->pos, b->pos) < a->radius + b->radius;
}
