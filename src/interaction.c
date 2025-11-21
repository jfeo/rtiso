#include "GLFW/glfw3.h"

#include "array.h"
#include "coord.h"
#include "interaction.h"

ARRAY_DEFINE(mouse_button_callback_fn, mbcbs)
ARRAY_DEFINE(scroll_callback_fn, scbs)
ARRAY_DEFINE(down_keys_callback_fn, dkcbs)
ARRAY_DEFINE(window_size_callback_fn, wscbs)

struct interaction interaction;

void key_callback(GLFWwindow *win, int key, int scan, int action, int mods) {
  switch (action) {
  case GLFW_PRESS:
    interaction.key_down[key] = 1;
    break;
  case GLFW_RELEASE:
    interaction.key_down[key] = 0;
    break;
  }
}

void interaction_mouse_button_callback(GLFWwindow *window, int button,
                                       int action, int mods) {
  for (int i = 0; i < interaction.mouse_button_callbacks.count; i++) {
    interaction.mouse_button_callbacks.elems[i](button, action, mods);
  }
}

void interaction_handle_down_keys() {
  for (int i = 0; i < interaction.down_keys_callbacks.count; i++) {
    interaction.down_keys_callbacks.elems[i](interaction.key_down);
  }
}

void interaction_window_size_callback(GLFWwindow *window, int width,
                                      int height) {
  for (int i = 0; i < interaction.window_size_callbacks.count; i++) {
    interaction.window_size_callbacks.elems[i](width, height);
  }
}

void interaction_scroll_callback(GLFWwindow *window, double xoffset,
                                 double yoffset) {
  for (int i = 0; i < interaction.scroll_callbacks.count; i++) {
    interaction.scroll_callbacks.elems[i](xoffset, yoffset);
  }
}

void interaction_init(GLFWwindow *window) {
  interaction.window = window;

  for (int i = 0; i < 256; i++) {
    interaction.key_down[i] = 0;
  }

  interaction.mouse_button_callbacks = array_mbcbs_init();
  interaction.scroll_callbacks = array_scbs_init();
  interaction.down_keys_callbacks = array_dkcbs_init();
  interaction.window_size_callbacks = array_wscbs_init();

  glfwSetWindowSizeCallback(window, *interaction_window_size_callback);
  glfwSetKeyCallback(window, *key_callback);
  glfwSetMouseButtonCallback(window, *interaction_mouse_button_callback);
  glfwSetScrollCallback(window, *interaction_scroll_callback);
}

void interaction_update() {
  glfwPollEvents();
  glfwGetCursorPos(interaction.window, &interaction.cursor_pos.x,
                   &interaction.cursor_pos.y);
  interaction_handle_down_keys();
}
