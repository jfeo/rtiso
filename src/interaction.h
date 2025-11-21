#ifndef _RTISO_INTERACTION_H_
#define _RTISO_INTERACTION_H_

#include "GLFW/glfw3.h"

#include "array.h"
#include "coord.h"

extern struct interaction interaction;

typedef void (*mouse_button_callback_fn)(int button, int action, int mods);
ARRAY_DECLARE(mouse_button_callback_fn, mbcbs)

typedef void (*scroll_callback_fn)(double xoffset, double yoffset);
ARRAY_DECLARE(scroll_callback_fn, scbs)

typedef void (*down_keys_callback_fn)(int key_down[256]);
ARRAY_DECLARE(down_keys_callback_fn, dkcbs)

typedef void (*window_size_callback_fn)(int width, int height);
ARRAY_DECLARE(window_size_callback_fn, wscbs)

struct interaction {
  GLFWwindow *window;
  array_mbcbs mouse_button_callbacks;
  array_scbs scroll_callbacks;
  array_dkcbs down_keys_callbacks;
  array_wscbs window_size_callbacks;

  int key_down[256];
  struct coord_window cursor_pos;
};

void interaction_init(GLFWwindow *window);

void interaction_update();

#endif /* ifndef _RTISO_INTERACTION_H_ */
