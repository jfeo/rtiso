#include <GL/glew.h>

#include "render_object.h"

#include "animation.h"
#include "coord.h"
#include "data.h"
#include "gl.h"
#include "linmath.h"
#include "shader.h"

struct render_object
render_object_create(const char *name, struct texture *tex,
                     struct shader_program *shader, GLsizei size_vertices,
                     GLfloat *vertices, GLsizei size_indices, GLuint *indices) {
  struct render_object obj = {.name = name, .tex = tex, .shader = shader};

  glGenVertexArrays(1, &obj.vao_id);
  glGenBuffers(1, &obj.vbo_id);
  glGenBuffers(1, &obj.ibo_id);

  glBindVertexArray(obj.vao_id);
  glBindBuffer(GL_ARRAY_BUFFER, obj.vbo_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ibo_id);

  shader_program_use(obj.shader);
  shader_program_enable_attr(obj.shader, "position");
  shader_program_enable_attr(obj.shader, "texcoord");

  glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_DYNAMIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, indices, GL_DYNAMIC_DRAW);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);

  return obj;
}

void render_object_update_animation(struct animation *anim, float tdiff) {
  animation_update(anim, tdiff);
  entity_vertices[3] = 0.5f / ((float)anim->frames) +
                       ((float)anim->current_frame) / ((float)anim->frames);
  entity_vertices[3 + 5] = 1.0f / ((float)anim->frames) +
                           ((float)anim->current_frame) / ((float)anim->frames);
  entity_vertices[3 + 5 * 2] =
      0.0f / ((float)anim->frames) +
      ((float)anim->current_frame) / ((float)anim->frames);
  entity_vertices[3 + 5 * 3] =
      0.5f / ((float)anim->frames) +
      ((float)anim->current_frame) / ((float)anim->frames);
  entity_vertices[3 + 5 * 4] =
      0.5f / ((float)anim->frames) +
      ((float)anim->current_frame) / ((float)anim->frames);
  entity_vertices[3 + 5 * 5] =
      1.0f / ((float)anim->frames) +
      ((float)anim->current_frame) / ((float)anim->frames);
  entity_vertices[3 + 5 * 6] =
      0.0f / ((float)anim->frames) +
      ((float)anim->current_frame) / ((float)anim->frames);
  glBufferData(GL_ARRAY_BUFFER, sizeof(entity_vertices), entity_vertices,
               GL_DYNAMIC_DRAW);
}
void render_object_draw(float tdiff, struct render_object *obj,
                        struct coord_real *pos) {
  mat4x4 mat_model;

  glBindVertexArray(obj->vao_id);
  glBindBuffer(GL_ARRAY_BUFFER, obj->vbo_id);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->ibo_id);
  glBindTexture(GL_TEXTURE_2D, obj->tex->id);
  gl_check_error("render object draw, bind texture");

  if (obj->anim != NULL) {
    render_object_update_animation(obj->anim, tdiff);
  }

  mat4x4_identity(mat_model);
  mat4x4_translate(mat_model, pos->nw, pos->up, pos->ne);
  shader_program_uniform_mat4(obj->shader, "model", 1, GL_FALSE, mat_model[0]);
  gl_check_error("render object draw, model uniform");

  glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
  gl_check_error("render object draw, elements");

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
