#ifndef __RTISO_UNIT_H__
#define __RTISO_UNIT_H__

#include "texture.h"
#include "coord.h"
#include "action.h"


struct unit {
	struct texture* tex;
	struct coord_real pos;
	struct action* action;
};

struct unit* unit_create(struct texture* tex, struct coord_real pos);
void unit_update(struct unit* unit);

#endif /* ifndef __RTISO_UNIT_H__ */
