#ifndef __RTISO_RENDERER_H__
#define __RTISO_RENDERER_H__

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "linmath.h"

#include "array.h"
#include "coord.h"
#include "entity.h"
#include "map.h"
#include "texture.h"

extern struct coord_window window_size;
extern struct coord_camera ortho_size;
extern struct coord_camera camera_pos;
extern struct coord_window tile_pixel_size;
extern struct coord_window mouse_pos;
extern GLfloat tile_world_size;

extern GLfloat tile_vdata[];
extern GLuint tile_vindices[];

ARRAY_DECLARE(struct entity *, entity)

void renderer_init(GLFWwindow *window, struct map *map);
void renderer_render(float tdiff, struct map *map, array_entity *entities);
void renderer_camera_zoom(float zoom_diff);
void renderer_camera_move(float x, float y);
void renderer_deinit();

#endif // ifndef __RTISO_RENDERER_H__
