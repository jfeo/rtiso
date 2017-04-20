#include "texture.h"

#include <SOIL/SOIL.h>

struct texture texture_create(const char* path) {
  unsigned char* image;
  struct texture tex;

  glGenTextures(1, &tex.id);
  glBindTexture(GL_TEXTURE_2D, tex.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  image = SOIL_load_image(path, &tex.w, &tex.h, 0, SOIL_LOAD_RGBA);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.w, tex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      image);
  glGenerateMipmap(GL_TEXTURE_2D);

  SOIL_free_image_data(image);

  glBindTexture(GL_TEXTURE_2D, 0); // unbind
  return tex;
}
