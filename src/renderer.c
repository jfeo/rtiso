#include "renderer.h"

#include <stdio.h>

void renderer_init() {
  glfwInit(); // initialize glfw

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(800, 600, "ctw", NULL, NULL); // Windowed
  glfwMakeContextCurrent(window); // select window

  // initialize glew
  glewExperimental = GL_TRUE;
  glewInit();

  glGenBuffers(1, &vertexBuffer);

  printf("%u\n", vertexBuffer);
}

void renderer_loop() {
  while(!glfwWindowShouldClose(window))
  {
      glfwSwapBuffers(window);
      glfwPollEvents();
  }
}

void renderer_deinit() {
  glfwTerminate(); // deinitialize glfw
}
