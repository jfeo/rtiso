#include "unit.h"

#include <stdlib.h>

struct unit* unit_create(struct texture* tex, struct phys_radial* phys) {
	struct unit* unit = (struct unit*)malloc(sizeof(struct unit));
	unit->entity = (struct entity*)malloc(sizeof(struct entity));
	unit->entity->tex = tex;
	unit->entity->phys = phys;
	unit->action = NULL;
	return unit;
}
