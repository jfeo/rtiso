#ifndef __RTISO_TEXTURE_H__
#define __RTISO_TEXTURE_H__

#include <GL/gl.h>

struct texture {
  GLuint id;
  int w, h;
};

struct texture texture_create(const char *path);

#endif // __RTISO_TEXTURE_H__
