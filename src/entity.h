#ifndef __RTIS_ENTITY_H__
#define __RTIS_ENTITY_H__

#include "coord.h"
#include "phys.h"
#include "render_object.h"
#include "texture.h"

struct entity {
  struct coord_real size;
  struct phys_radial *phys;
  struct render_object *render_obj;
};

#endif /* ifndef __RTIS_ENTITY_H__ */
