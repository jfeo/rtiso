#ifndef __RTISO_SPRITE_H__
#define __RTISO_SPRITE_H__

#include <GL/glew.h>

#include "linmath.h"
/* #include "texture.h" */

typedef struct {
  /* texture_t texture; */
  GLfloat x, y, w, h;
  mat4x4 model;
  GLuint count;
  GLuint vbo_offset;
} sprite_t;

GLuint sprite_vbo, sprite_vao, sprite_shader;
GLint sprite_shader_attrib_pos, sprite_shader_attrib_col;
GLint sprite_shader_uniform_proj, sprite_shader_uniform_view, sprite_shader_uniform_model;

static const int sprite_space_increment = 16;
extern int sprite_count;
extern int sprite_space;
extern GLfloat* sprite_vdata;
mat4x4 sprite_proj;
mat4x4 sprite_view;

/**
 * Initialize the sprite sub-system.
 */
void sprite_init();

/**
 *
 */
void sprite_vdata_alloc();

/**
 * Returns the pointer to a dynamically allocated sprite struct.
 */
sprite_t* sprite_create(GLfloat x, GLfloat y, GLfloat w,  GLfloat h);

/**
 * Draw the given sprite.
 */
void sprite_draw(sprite_t* sprite);

/**
 * Free the given sprite.
 */
void sprite_delete(sprite_t* sprite);

#endif /* ifndef __RTISO_SPRITE_H__ */
