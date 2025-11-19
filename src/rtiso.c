/*
 * file: main.c
 * author: jfeo (jens@feodor.dk)
 * date: 17/08/2016
 */
#include "renderer.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "animation.h"
#include "map.h"
#include "phys.h"
#include "unit.h"

struct map map;
int key_down[256];

struct coord_real to = {.ne = 30.0f, .nw = 15.0f};
struct unit *moving_unit;
struct unit *static_unit;

void key_callback(GLFWwindow *win, int key, int scan, int action, int mods) {
  switch (action) {
  case GLFW_PRESS:
    key_down[key] = 1;
    break;
  case GLFW_RELEASE:
    key_down[key] = 0;
    break;
  }
}

void window_size_callback(GLFWwindow *window, int width, int height) {
  window_size.x = width;
  window_size.y = height;
  glViewport(0, 0, width, height);
  renderer_update_projection();
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  struct coord_window mpos;
  struct coord_camera mpos_cam;
  struct coord_tile mpos_tile;
  struct coord_real mpos_real;
  glfwGetCursorPos(window, &mpos.x, &mpos.y);
  mpos_cam = coord_camera_from_window(mpos);
  mpos_tile = coord_tile_from_camera(mpos_cam, 0);
  mpos_real = coord_real_from_camera(mpos_cam, 0.0f);
  switch (action) {
  case GLFW_PRESS:
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
      printf("Coord Window: %f %f\n", mpos.x, mpos.y);
      printf("Coord Camera: %f %f\n", mpos_cam.x, mpos_cam.y);
      printf("Coord Tile:   %d %d\n", mpos_tile.ne, mpos_tile.nw);
      printf("Coord Real:   %f %f\n", mpos_real.ne, mpos_real.nw);
      printf("Camera Pos:   %f %f\n", camera_pos.x, camera_pos.y);
      break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      moving_unit->action->data.move.to.ne = mpos_real.ne;
      moving_unit->action->data.move.to.nw = mpos_real.nw;
      break;
    }
    break;
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  renderer_camera_zoom((float)yoffset);
}

void handle_down_keys() {
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
    renderer_camera_zoom(0.1);
  }
  if (key_down[GLFW_KEY_E]) {
    renderer_camera_zoom(-0.1);
  }
}

void move_unit(struct unit *moving_unit, struct coord_real move_diff,
               float ms) {
  float move_dist;

  if (moving_unit->action->type != MOVE) {
    return;
  }

  move_diff.ne =
      moving_unit->action->data.move.to.ne - moving_unit->entity->phys->pos.ne;
  move_diff.nw =
      moving_unit->action->data.move.to.nw - moving_unit->entity->phys->pos.nw;
  move_dist = 1.0f / sqrt(powf(move_diff.ne, 2.0f) + powf(move_diff.nw, 2.0f));

  if (pow(move_diff.ne, 2.0f) < pow(move_dist, 2.0f)) {
    moving_unit->entity->phys->pos.ne = moving_unit->action->data.move.to.ne;
  } else {
    moving_unit->entity->phys->pos.ne += move_dist * move_diff.ne * ms / 100.0f;
  }

  if (pow(move_diff.nw, 2.0f) < pow(move_dist, 2.0f)) {
    moving_unit->entity->phys->pos.nw = moving_unit->action->data.move.to.nw;
  } else {
    moving_unit->entity->phys->pos.nw += move_dist * move_diff.nw * ms / 100.0f;
  }
}

int main(int argc, char *argv[]) {
  glfwInit(); // initialize glfw

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(window_size.x, window_size.y, "rtiso",
                                        NULL, NULL); // Windowed
  glfwMakeContextCurrent(window);                    // select window

  // initialize glew
  glewExperimental = GL_TRUE;
  glewInit();

  for (int i = 0; i < 256; i++) {
    key_down[i] = 0;
  }

  map = map_init(256, 256);

  renderer_init(window, &map);

  glfwSetWindowSizeCallback(window, *window_size_callback);
  glfwSetKeyCallback(window, *key_callback);
  glfwSetMouseButtonCallback(window, *mouse_button_callback);
  glfwSetScrollCallback(window, *scroll_callback);

  struct texture texture_unit = texture_create("assets/tex/units.png");
  struct coord_real moving_unit_start = {.nw = 1.0f, .up = 0.1f, .ne = 1.0f};
  moving_unit =
      unit_create(&texture_unit, phys_radial_create(moving_unit_start));
  moving_unit->entity->anim = animation_create(&texture_unit, 8, 30.0f, 32);
  moving_unit->action = (struct action *)malloc(sizeof(struct action));
  moving_unit->action->type = MOVE;
  moving_unit->action->data.move.to = to;

  struct coord_real static_unit_pos = {.nw = 50.0f, .up = 0.0f, .ne = 50.0f};
  static_unit = unit_create(&texture_unit, phys_radial_create(static_unit_pos));
  static_unit->entity->anim = animation_create(&texture_unit, 8, 100.0f, 32);
  struct timeval tv_before;
  struct timeval tv_after;
  struct coord_real move_diff;
  double ms = 0.0f;

  array_entity entities = array_entity_init();
  array_entity_add(&entities, moving_unit->entity);
  array_entity_add(&entities, static_unit->entity);

  while (!glfwWindowShouldClose(window)) {
    gettimeofday(&tv_before, NULL);
    renderer_render(ms, &map, &entities);
    glfwPollEvents();
    handle_down_keys();
    move_unit(moving_unit, move_diff, ms);
    gettimeofday(&tv_after, NULL);
    ms = (tv_after.tv_sec - tv_before.tv_sec) * 1000.0f +
         (tv_after.tv_usec - tv_before.tv_usec) / 1000.0f;
  }

  renderer_deinit();

  return EXIT_SUCCESS;
}
