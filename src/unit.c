#include "unit.h"

#include <stdlib.h>

struct unit* unit_create(struct texture* tex, struct coord_real pos) {
	struct unit* unit = (struct unit*)malloc(sizeof(struct unit));
	unit->tex = tex;
	unit->pos = pos;
	unit->action = NULL;
	return unit;
}
