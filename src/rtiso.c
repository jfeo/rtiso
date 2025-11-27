/*
 * file: main.c
 * author: jfeo (jens@feodor.dk)
 * date: 17/08/2016
 */
#include "action.h"
#include "animation.h"
#include "coord.h"
#include "renderer.h"

#include "data.h"
#include "render_object.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "interaction.h"
#include "map.h"
#include "phys.h"
#include "shader.h"
#include "time.h"
#include "timer.h"
#include "unit.h"

int quit = 0;
struct map map;
struct coord_real to = {.ne = 30.0f, .nw = 15.0f};
struct unit *moving_unit;
struct unit *following_unit;
double scroll_zoom_sensitivity = 100.0f;

struct render_object following_robj;
struct render_object moving_robj;
struct texture texture_unit;
array_entity entities;

void mouse_button_callback(int button, int action, int mods) {
  struct coord_window cursor_pos = interaction.cursor_pos;
  struct coord_camera cursor_pos_cam;
  struct coord_tile cursor_pos_tile;
  struct coord_real cursor_pos_real;
  cursor_pos_cam = coord_camera_from_window(interaction.cursor_pos);
  cursor_pos_tile = coord_tile_from_camera(cursor_pos_cam, 0);
  cursor_pos_real = coord_real_from_camera(cursor_pos_cam, 0.0f);
  switch (action) {
  case GLFW_PRESS:
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
      printf("Coord Window: %f %f\n", cursor_pos.x, cursor_pos.y);
      printf("Coord Camera: %f %f\n", cursor_pos_cam.x, cursor_pos_cam.y);
      printf("Coord Tile:   %d %d\n", cursor_pos_tile.ne, cursor_pos_tile.nw);
      printf("Coord Real:   %f %f\n", cursor_pos_real.ne, cursor_pos_real.nw);
      printf("Camera Pos:   %f %f\n", camera_pos.x, camera_pos.y);
      break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      moving_unit->action.type = ACTION_MOVE;
      moving_unit->action.data.move.to.ne = cursor_pos_real.ne;
      moving_unit->action.data.move.to.nw = cursor_pos_real.nw;
      break;
    }
    break;
  }
}

void scroll_callback(double xoffset, double yoffset) {
  renderer_camera_zoom((float)yoffset / scroll_zoom_sensitivity);
}

void handle_down_keys(int *key_down) {
  if (key_down[GLFW_KEY_A]) {
    renderer_camera_move(2.0f, 0.0f);
  }
  if (key_down[GLFW_KEY_D]) {
    renderer_camera_move(-2.0f, 0.0f);
  }
  if (key_down[GLFW_KEY_W]) {
    renderer_camera_move(0.0f, -2.0f);
  }
  if (key_down[GLFW_KEY_S]) {
    renderer_camera_move(0.0f, 2.0f);
  }
  if (key_down[GLFW_KEY_Q]) {
    quit = 1;
  }
  if (key_down[GLFW_KEY_E]) {
    renderer_camera_zoom(-0.1);
  }
}

void unit_follow_cursor_update() {
  struct coord_real cursor_real =
      coord_real_from_window(interaction.cursor_pos, 0.0);
  following_unit->action.type = ACTION_MOVE;
  following_unit->action.data.move.to = cursor_real;
}

GLFWwindow *create_window() {
  glfwInit(); // initialize glfw

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(window_size.x, window_size.y, "rtiso", NULL, NULL);
  glfwMakeContextCurrent(window);

  return window;
}

void create_units() {
  texture_unit = texture_create("assets/tex/units.png");
  struct coord_real moving_unit_start = {.nw = 1.0f, .up = 0.0f, .ne = 1.0f};
  moving_robj = render_object_create(
      "moving unit", &texture_unit, &world_shader, sizeof(entity_vertices),
      entity_vertices, sizeof(entity_indices), entity_indices);
  moving_unit =
      unit_create(&moving_robj, phys_radial_create(moving_unit_start));
  moving_unit->entity->render_obj->anim =
      animation_create(&texture_unit, 8, 30.0f, 32);
  moving_unit->action.type = ACTION_MOVE;
  moving_unit->action.data.move.to = to;

  struct coord_real following_unit_pos = {.nw = 50.0f, .up = 0.0f, .ne = 50.0f};
  following_robj = render_object_create(
      "following unit", &texture_unit, &world_shader, sizeof(entity_vertices),
      entity_vertices, sizeof(entity_indices), entity_indices);
  following_unit =
      unit_create(&following_robj, phys_radial_create(following_unit_pos));
  following_unit->entity->render_obj->anim =
      animation_create(&texture_unit, 8, 100.0f, 32);
  struct coord_real move_diff;

  entities = array_entity_init();
  array_entity_add(&entities, moving_unit->entity);
  array_entity_add(&entities, following_unit->entity);
}

int main(int argc, char *argv[]) {
  GLFWwindow *window = create_window();

  printf("initializing interaction system\n");
  interaction_init(window);

  printf("initializing map\n");
  map = map_init(256, 256);

  printf("initializing renderer system\n");
  renderer_init(window, &map);

  printf("initializing physics system\n");
  phys_init();

  array_scbs_add(&interaction.scroll_callbacks, &scroll_callback);
  array_dkcbs_add(&interaction.down_keys_callbacks, &handle_down_keys);
  array_mbcbs_add(&interaction.mouse_button_callbacks, &mouse_button_callback);

  units_init();
  create_units();

  struct timer timer = timer_init();
  double ms;

  printf("entering engine loop\n");
  while (!quit && !glfwWindowShouldClose(window)) {
    ms = timer_elapsed_ms(&timer);
    interaction_update();
    renderer_render(ms, &map, &entities);
    phys_update(ms);
    unit_follow_cursor_update();
    units_update(ms);
  }

  renderer_deinit();

  return EXIT_SUCCESS;
}
