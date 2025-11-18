#ifndef __RTISO_MAP_H__
#define __RTISO_MAP_H__

#include "GL/gl.h"
#include "coord.h"

struct tile {
  struct coord_tile pos;
  int type;
  int subtype;
};

struct map {
  struct coord_tile size;
  struct tile *tiles;
  GLfloat *vertices;
  GLfloat *texcoords;
  GLuint *indices;
};

struct map map_init(unsigned int west, unsigned int height);

#endif /* ifndef __RTISO_MAP_H__ */
