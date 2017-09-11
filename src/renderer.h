#ifndef __RTISO_RENDERER_H__
#define __RTISO_RENDERER_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "linmath.h"

#include "coord.h"
#include "map.h"
#include "texture.h"
#include "entity.h"

#define RTISO_2_PI 2 * acos(-1.0)
#define RTISO_PI acos(-1.0)
#define SQRT_2 1.414213562373095048801688724209698078569671875376948073176f

/**
 * Check GL for error
 */
GLint renderer_check_gl_error(const char* msg);

extern struct coord_window window_size;
extern struct coord_camera ortho_size;
extern struct coord_camera camera_pos;
extern struct coord_window tile_pixel_size;
extern struct coord_window mouse_pos;
extern GLfloat tile_world_size;

extern GLfloat tile_vdata[];
extern GLuint tile_vindices[];

GLuint vao_tiles, vbo_tiles, ibo_tiles, vao_entity, vbo_entity, ibo_entity;
GLuint shader;
GLint shader_attrib_pos, shader_attrib_tex, shader_attrib_tile_pos, shader_attrib_tile_type, shader_attrib_tile_subtype;
GLuint shader_mat_model, shader_mat_view, shader_mat_proj, shader_mat_camera;

GLuint vao_text, vbo_text, tex_text;
GLint shader_text;
GLuint shader_text_attrib_pos, shader_text_attrib_tex, shader_text_color, shader_text_mat_proj;
FT_Library ft;
FT_Face face;
FT_GlyphSlot g;

struct texture texture;

/*
 * Initializes GLFW and GLEW and creates a GL context.
 */

GLFWwindow* window;

void renderer_init(GLFWwindow* window, struct map* map);

void renderer_draw_map(struct map* map);

void renderer_render(float tdiff, struct map* map, struct entity* test_entity);

void renderer_camera_move(float x, float y);

void renderer_camera_update();

void renderer_update_projection();

void renderer_draw_animation(struct animation* anim, float tdiff);

void renderer_draw_entity(struct entity* entity, float tdiff);

void renderer_draw_text(const char *text, float x, float y);

void renderer_update_map(struct map* map);

/**
 * Terminate GLFW.
 */
void renderer_deinit();

void printm(mat4x4 m);

#endif // ifndef __RTISO_RENDERER_H__
