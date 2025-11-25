#include <GL/glew.h>

#include "tilemap.h"

#include "data.h"
#include "gl.h"
#include "shader.h"
#include "texture.h"

GLuint vao_tiles, vbo_tiles, ibo_tiles, vbo_map;
struct texture tiles_tex;

void tilemap_update(struct map *map) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo_tiles);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(struct tile) * map->size.nw * map->size.ne, map->tiles,
               GL_STREAM_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void tilemap_init(struct map *map) {
  shader_program_use(&world_shader);

  tiles_tex = texture_create("assets/tex/tiles.png");
  gl_check_error("tiles texture creation");
  glBindTexture(GL_TEXTURE_2D, tiles_tex.id);

  // vao, vbo_tiles, ibo_tiles, etc.
  glGenVertexArrays(1, &vao_tiles);
  glGenBuffers(1, &vbo_tiles);
  glGenBuffers(1, &ibo_tiles);
  glGenBuffers(1, &vbo_map);

  glBindVertexArray(vao_tiles);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_tiles);
  shader_program_enable_attr(&world_shader, "position");
  shader_program_enable_attr(&world_shader, "texcoord");
  glBufferData(GL_ARRAY_BUFFER, sizeof(tile_vertices), tile_vertices,
               GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_tiles);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tile_indices), tile_indices,
               GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_map);
  shader_program_enable_attr(&world_shader, "tile_pos");
  shader_program_enable_attr(&world_shader, "tile_type");
  shader_program_enable_attr(&world_shader, "tile_subtype");
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(struct tile) * map->size.nw * map->size.ne, map->tiles,
               GL_STREAM_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0); // unbind vao_tiles
}

void tilemap_draw(struct map *map) {
  glBindVertexArray(vao_tiles);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_tiles);
  glBindTexture(GL_TEXTURE_2D, tiles_tex.id);
  gl_check_error("draw map bind");

  shader_program_use(&world_shader);

  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0,
                          map->size.nw * map->size.ne);

  gl_check_error("draw map, elements instanced");

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}
