#ifndef __RTIS_ENTITY_H__
#define __RTIS_ENTITY_H__

#include "phys.h"
#include "texture.h"
#include "coord.h"

struct entity {
	struct coord_real size;
	struct phys_radial* phys;
	struct texture* tex;
	struct animation* anim;
};

#endif /* ifndef __RTIS_ENTITY_H__ */
