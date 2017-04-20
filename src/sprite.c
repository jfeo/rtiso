#include "sprite.h"

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

int sprite_space;
int sprite_count;
GLfloat* sprite_vdata = NULL;

void sprite_init() {
  // variable definition
  sprite_count = 0;
  sprite_space = 0;

  // gl stuff
  glGenVertexArrays(1, &sprite_vao);
  glBindVertexArray(sprite_vao);
  glGenBuffers(1, &sprite_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);

  // allocate initial sprite space
  sprite_vdata_alloc();

  // allocate space for sprite vertex data
  sprite_vdata = (GLfloat*)malloc(5 * sprite_space * sizeof(GLfloat));

  // create sprite shader
  GLuint vshader = shader_create_from_file(GL_VERTEX_SHADER, "res/shaders/sprite.vert.glsl");
  GLuint fshader = shader_create_from_file(GL_FRAGMENT_SHADER, "res/shaders/sprite.frag.glsl");
  sprite_shader = shader_program(vshader, fshader);
  glUseProgram(sprite_shader);
  sprite_shader_attrib_pos = glGetAttribLocation(sprite_shader, "position");
  glEnableVertexAttribArray(sprite_shader_attrib_pos);
  glVertexAttribPointer(sprite_shader_attrib_pos, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), 0);
  sprite_shader_attrib_col = glGetAttribLocation(sprite_shader, "color");
  glEnableVertexAttribArray(sprite_shader_attrib_col);
  glVertexAttribPointer(sprite_shader_attrib_col, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (GLvoid*)(2 * sizeof(GLfloat)));

  mat4x4_ortho(sprite_proj, 0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 1.0f);
  mat4x4_translate(sprite_view, 0.9f, 0.0f, 0.0f);
  /* mat4x4_translate(sprite_view, -4.0f, -3.0f, 0.0f); */
  sprite_shader_uniform_proj = glGetUniformLocation(sprite_shader, "proj");
  sprite_shader_uniform_view = glGetUniformLocation(sprite_shader, "view");
  sprite_shader_uniform_model = glGetUniformLocation(sprite_shader, "model");
  glUniformMatrix4fv(sprite_shader_uniform_proj, 1, GL_FALSE, &sprite_proj[0][0]);
  glUniformMatrix4fv(sprite_shader_uniform_view, 1, GL_FALSE, &sprite_view[0][0]);
}


void sprite_vdata_alloc() {
  sprite_space += sprite_space_increment;
  sprite_vdata = (GLfloat*)realloc(sprite_vdata, 6 * 5 * sprite_space * sizeof(GLfloat));
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 5 * 6 * sprite_space, sprite_vdata, GL_DYNAMIC_DRAW);
}

sprite_t* sprite_create(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
  sprite_t* sprite = (sprite_t*)malloc(sizeof(sprite_t));
  sprite->x = x;
  sprite->y = y;
  sprite->w = w;
  sprite->h = h;
  sprite->count = sprite_count;
  sprite->vbo_offset = sprite_count * 5 * 6;
  mat4x4_identity(sprite->model);

  if (sprite_space == sprite_count) {
    printf("info: allocating more vertex buffer space\n");
    sprite_vdata_alloc();
  }

  sprite_count++;

  printf("sprite vbo_offset = %d\n", sprite->vbo_offset);

  sprite_vdata[sprite->vbo_offset] = x;
  sprite_vdata[sprite->vbo_offset + 1] = y;
  sprite_vdata[sprite->vbo_offset + 2] = 1.0f;
  sprite_vdata[sprite->vbo_offset + 3] = 0.0f;
  sprite_vdata[sprite->vbo_offset + 4] = 0.0f;
  sprite_vdata[sprite->vbo_offset + 5] = x;
  sprite_vdata[sprite->vbo_offset + 6] = y + h;
  sprite_vdata[sprite->vbo_offset + 7] = 0.0f;
  sprite_vdata[sprite->vbo_offset + 8] = 1.0f;
  sprite_vdata[sprite->vbo_offset + 9] = 0.0f;
  sprite_vdata[sprite->vbo_offset + 10] = x + w;
  sprite_vdata[sprite->vbo_offset + 11] = y;
  sprite_vdata[sprite->vbo_offset + 12] = 1.0f;
  sprite_vdata[sprite->vbo_offset + 13] = 1.0f;
  sprite_vdata[sprite->vbo_offset + 14] = 1.0f;
  sprite_vdata[sprite->vbo_offset + 15] = x + w;
  sprite_vdata[sprite->vbo_offset + 16] = y + h;
  sprite_vdata[sprite->vbo_offset + 17] = 0.0f;
  sprite_vdata[sprite->vbo_offset + 18] = 0.0f;
  sprite_vdata[sprite->vbo_offset + 19] = 1.0f;
  sprite_vdata[sprite->vbo_offset + 20] = x;
  sprite_vdata[sprite->vbo_offset + 21] = y + h;
  sprite_vdata[sprite->vbo_offset + 22] = 0.0f;
  sprite_vdata[sprite->vbo_offset + 23] = 1.0f;
  sprite_vdata[sprite->vbo_offset + 24] = 0.0f;
  sprite_vdata[sprite->vbo_offset + 25] = x + w;
  sprite_vdata[sprite->vbo_offset + 26] = y;
  sprite_vdata[sprite->vbo_offset + 27] = 1.0f;
  sprite_vdata[sprite->vbo_offset + 28] = 1.0f;
  sprite_vdata[sprite->vbo_offset + 29] = 1.0f;

  glBufferSubData(GL_ARRAY_BUFFER, sprite->vbo_offset * sizeof(GLfloat),
      sizeof(GLfloat) * 5 * 6, &sprite_vdata[sprite->vbo_offset]);

  return sprite;
}

void sprite_draw(sprite_t* sprite) {
  glBindVertexArray(sprite_vao);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * sprite->vbo_offset,  sizeof(GLfloat) * 5 * 6,
      &sprite_vdata[sprite->vbo_offset]);
  glDrawArrays(GL_TRIANGLES, sprite->count * 6, 6);
  glBindVertexArray(0);
  glUniformMatrix4fv(sprite_shader_uniform_model, 1, GL_FALSE, &(sprite->model[0][0]));
}
