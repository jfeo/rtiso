#ifndef __RTISO_PROJECTILE_H__
#define __RTISO_PROJECTILE_H__

#include "phys.h"

struct projectile {
  struct phys_radial phys;
};

struct projectile projectile_create();

#endif /* ifndef __RTISO_PROJECTILE_H__ */
