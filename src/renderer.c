#include <GL/glew.h>
#include <stdio.h>

#include "renderer.h"

#include "array.h"
#include "constants.h"
#include "coord.h"
#include "gl.h"
#include "interaction.h"
#include "linmath.h"
#include "render_object.h"
#include "shader.h"
#include "text.h"
#include "tilemap.h"

ARRAY_DEFINE(struct entity *, entity)

float camera_zoom = 2.0;
struct coord_window window_size = {.x = 800.0f, .y = 600.0f};
struct coord_camera ortho_size;
struct coord_camera camera_pos = {.x = 0.0f, .y = 0.0f};
struct coord_window tile_pixel_size = {.x = 16.0f, .y = 8.0f};
struct coord_window mouse_pos = {.x = 0.0f, .y = 0.0f};

struct shader_program world_shader;

GLFWwindow *window;

mat4x4 model, model_entity_trans, model_entity_rot;
mat4x4 view;
mat4x4 proj;
mat4x4 camera;

vec3 pos = {0.0f, 0.0f, 0.0f};
vec3 front = {0.0f, 0.0f, 1.0f};
vec3 up = {0.0f, 1.0f, 0.0f};

void renderer_update_projection() {
  shader_program_use(&world_shader);

  double scale = pow(2.0, camera_zoom);

  float aspect = window_size.y / window_size.x;
  float screen_tiles = window_size.x / tile_pixel_size.x;
  ortho_size.x = screen_tiles * SQRT_2;
  ortho_size.y = ortho_size.x * aspect;
  mat4x4_ortho(proj, -ortho_size.x / scale, ortho_size.x / scale,
               -ortho_size.y / scale, ortho_size.y / scale, 0.0f, 1000.0f);

  shader_program_uniform_mat4(&world_shader, "proj", 1, GL_FALSE,
                              (GLfloat *)proj);

  text_update_projection(window_size);
}

void renderer_window_size_callback(int width, int height) {
  window_size.x = width;
  window_size.y = height;
  glViewport(0, 0, width, height);
  renderer_update_projection();
}

void renderer_init(GLFWwindow *win, struct map *map) {
  window = win;

  array_wscbs_add(&interaction.window_size_callbacks,
                  &renderer_window_size_callback);

  // initialize glew
  glewExperimental = GL_TRUE;
  glewInit();

  text_init();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // set up matrices
  mat4x4_identity(camera);
  mat4x4_identity(model);
  mat4x4_look_at(view, pos, front, up);
  mat4x4_rotate_X(view, view, -RTISO_PI / 6);
  mat4x4_rotate_Y(view, view, RTISO_2_PI / 8);

  world_shader = shader_program_from_file("assets/shaders/simple.vert.glsl",
                                          "assets/shaders/simple.frag.glsl",
                                          "world-shader");

  shader_program_add_attr(&world_shader,
                          (struct shader_attr){.name = "position",
                                               .size = 3,
                                               .type = GL_FLOAT,
                                               .normalized = GL_FALSE,
                                               .stride = 5 * sizeof(GLfloat),
                                               .pointer = 0});
  shader_program_add_attr(
      &world_shader,
      (struct shader_attr){.name = "texcoord",
                           .size = 2,
                           .type = GL_FLOAT,
                           .normalized = GL_FALSE,
                           .stride = 5 * sizeof(GLfloat),
                           .pointer = (GLvoid *)(3 * sizeof(GLfloat))});

  shader_program_add_attr(&world_shader,
                          (struct shader_attr){.name = "tile_pos",
                                               .size = 3,
                                               .type = GL_INT,
                                               .stride = sizeof(struct tile),
                                               .pointer = 0,
                                               .divisor = 1});

  shader_program_add_attr(
      &world_shader,
      (struct shader_attr){.name = "tile_type",
                           .size = 1,
                           .type = GL_INT,
                           .stride = sizeof(struct tile),
                           .pointer = (GLvoid *)(sizeof(struct coord_tile)),
                           .divisor = 1});

  shader_program_add_attr(
      &world_shader,
      (struct shader_attr){
          .name = "tile_subtype",
          .size = 1,
          .type = GL_INT,
          .stride = sizeof(struct tile),
          .pointer = (GLvoid *)(sizeof(struct coord_tile) + sizeof(int)),
          .divisor = 1});

  shader_program_use(&world_shader);
  shader_program_add_uniform(&world_shader,
                             (struct shader_uniform){.name = "model"});
  shader_program_add_uniform(&world_shader,
                             (struct shader_uniform){.name = "view"});
  shader_program_add_uniform(&world_shader,
                             (struct shader_uniform){.name = "proj"});
  shader_program_add_uniform(&world_shader,
                             (struct shader_uniform){.name = "camera"});

  printf("sending world shader uniform data\n");
  shader_program_uniform_mat4(&world_shader, "view", 1, GL_FALSE,
                              (GLfloat *)view);
  shader_program_uniform_mat4(&world_shader, "model", 1, GL_FALSE,
                              (GLfloat *)model);
  shader_program_uniform_mat4(&world_shader, "camera", 1, GL_FALSE,
                              (GLfloat *)camera);

  tilemap_init(map);

  renderer_update_projection();
  gl_check_error("update proj");

  mat4x4_identity(model_entity_trans);
  mat4x4_identity(model_entity_rot);
}

void renderer_draw_entity(struct entity *entity, float tdiff) {
  render_object_draw(tdiff, entity->render_obj, &entity->phys->pos);
}

void renderer_draw_entities(array_entity *entities, float tdiff) {
  for (int i = 0; i < entities->count; i++) {
    struct entity *entity = entities->elems[i];
    renderer_draw_entity(entity, tdiff);
  }
}

void renderer_camera_move(float x, float y) {
  camera_pos.x += x;
  camera_pos.y += y;
}

void renderer_camera_zoom(float zoom_diff) {
  if (zoom_diff > 0 && camera_zoom + zoom_diff > 1000.0f) {
    camera_zoom = 1000.0f;
  } else {
    camera_zoom += zoom_diff;
  }

  renderer_update_projection();
}

void renderer_camera_update() {
  shader_program_use(&world_shader);
  mat4x4_translate(camera, camera_pos.x, camera_pos.y, 0);

  shader_program_uniform_mat4(&world_shader, "camera", 1, GL_FALSE, camera[0]);

  glUseProgram(0);
}

void renderer_draw_debug_text(float tdiff) {
  struct coord_tile mpos_tile;
  struct coord_camera mpos_cam;
  struct coord_window mpos_win;
  struct coord_real mpos_real;

  glfwGetCursorPos(window, &mpos_win.x, &mpos_win.y);
  mpos_cam = coord_camera_from_window(mpos_win);
  mpos_tile = coord_tile_from_camera(mpos_cam, 0.0f);
  mpos_real = coord_real_from_camera(mpos_cam, 0.0f);
  char cstr[64], mstr_w[64], mstr_c[64], mstr_t[64], mstr_r[64], fpsstr[64];

  sprintf(fpsstr, "FPS: %0.3f", 1000.0f / tdiff);
  sprintf(mstr_w, "Mouse (window): %0.2f %0.2f", mpos_win.x, mpos_win.y);
  sprintf(mstr_c, "Mouse (camera): %0.2f %0.2f", mpos_cam.x, mpos_cam.y);
  sprintf(mstr_t, "Mouse (tile): %d %d", mpos_tile.nw, mpos_tile.ne);
  sprintf(mstr_r, "Mouse (real): %0.2f %0.2f", mpos_real.nw, mpos_real.ne);
  sprintf(cstr, "Camera: %0.0f %0.0f (%0.2f)", camera_pos.x, camera_pos.y,
          camera_zoom);
  text_draw(fpsstr, 5.0f, window_size.y - 19.0f);
  text_draw(mstr_w, 5.0f, 24.0f);
  text_draw(mstr_c, 5.0f, 43.0f);
  text_draw(mstr_t, 5.0f, 62.0f);
  text_draw(mstr_r, 5.0f, 81.0f);
  text_draw(cstr, 5.0f, 5.0f);
}

void renderer_update_model() {
  shader_program_use(&world_shader);
  shader_program_uniform_mat4(&world_shader, "model", 1, GL_FALSE,
                              (GLfloat *)model);
}

void renderer_render(float tdiff, struct map *map, array_entity *entities) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderer_update_model();
  renderer_camera_update();
  tilemap_draw(map);
  renderer_draw_entities(entities, tdiff);
  renderer_draw_debug_text(tdiff);

  glfwSwapBuffers(window);
}

void renderer_deinit() {
  glfwTerminate(); // deinitialize glfw
}
