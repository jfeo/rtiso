#ifndef _RTISO_RENDER_OBJECT_H_
#define _RTISO_RENDER_OBJECT_H_

#include "coord.h"
#include "linmath.h"
#include "shader.h"
#include "texture.h"

struct render_object {
  const char *name;
  GLuint vao_id;
  GLuint vbo_id;
  GLuint ibo_id;

  struct shader_program *shader;
  struct texture *tex;
  struct animation *anim;
};

struct render_object
render_object_create(const char *name, struct texture *tex,
                     struct shader_program *shader, GLsizei size_vertices,
                     GLfloat *vertices, GLsizei size_indices, GLuint *indices);

void render_object_draw(float tdiff, struct render_object *obj,
                        struct coord_real *pos);

#endif /* ifndef _RTISO_RENDER_OBJECT_H_ */
