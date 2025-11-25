#include "freetype2/freetype/freetype.h"
#include <GL/glew.h>

#include "text.h"

#include "coord.h"
#include "linmath.h"
#include "shader.h"

FT_Library ft;
FT_Face face;
FT_GlyphSlot g;

struct shader_program text_shader;
mat4x4 text_proj;

GLuint text_vao_id, text_vbo_id, text_tex_id;

void text_init() {
  if (FT_Init_FreeType(&ft)) {
    printf("Could not init freetype library\n");
    return;
  }

  if (FT_New_Face(ft, "assets/DejaVuSans.ttf", 0, &face)) {
    printf("Could not open font\n");
    return;
  }

  FT_Set_Pixel_Sizes(face, 0, 14);
  g = face->glyph;

  text_shader =
      shader_program_from_file("assets/shaders/text.vert.glsl",
                               "assets/shaders/text.frag.glsl", "text-shader");
  shader_program_add_attr(&text_shader, (struct shader_attr){
                                            .name = "pos",
                                            .size = 4,
                                            .type = GL_FLOAT,
                                            .normalized = GL_FALSE,
                                            .stride = 0,
                                            .pointer = 0,
                                        });

  shader_program_use(&text_shader);
  shader_program_add_uniform(&text_shader,
                             (struct shader_uniform){.name = "textColor"});
  shader_program_add_uniform(&text_shader,
                             (struct shader_uniform){.name = "proj"});

  GLfloat color_white[3] = {1, 1, 1};
  shader_program_uniform_vec3(&text_shader, "textColor", 1, color_white);
  shader_program_uniform_mat4(&text_shader, "proj", 1, GL_FALSE,
                              (GLfloat *)text_proj);

  // create text vao, vbo, texture
  glGenVertexArrays(1, &text_vao_id);
  glGenBuffers(1, &text_vbo_id);
  glGenTextures(1, &text_tex_id);
  glBindVertexArray(text_vao_id);
  glBindBuffer(GL_ARRAY_BUFFER, text_vbo_id);
  shader_program_enable_attr(&text_shader, "pos");

  glBindTexture(GL_TEXTURE_2D, text_tex_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindVertexArray(0);
}

void text_update_projection(struct coord_window window_size) {
  shader_program_use(&text_shader);
  mat4x4_ortho(text_proj, 0, window_size.x, 0, window_size.y, 0, 1.0f);
  shader_program_uniform_mat4(&text_shader, "proj", 1, GL_FALSE,
                              (GLfloat *)text_proj);
}

void text_draw(const char *text, float x, float y) {
  float sx = 1.0f;
  float sy = 1.0f;

  glBindVertexArray(text_vao_id);
  glBindBuffer(GL_ARRAY_BUFFER, text_vbo_id);
  glBindTexture(GL_TEXTURE_2D, text_tex_id);
  shader_program_use(&text_shader);

  const char *p;
  for (p = text; *p; p++) {
    if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
      continue;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0,
                 GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

    float x2 = x + g->bitmap_left * sx;
    float y2 = -y - g->bitmap_top * sy;
    float w = g->bitmap.width * sx;
    float h = g->bitmap.rows * sy;

    GLfloat box[4][4] = {
        {x2, -y2, 0, 0},
        {x2 + w, -y2, 1, 0},
        {x2, -y2 - h, 0, 1},
        {x2 + w, -y2 - h, 1, 1},
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    x += (g->advance.x / 64.0) * sx;
    y += (g->advance.y / 64.0) * sy;
  }
  glBindVertexArray(0);
}
