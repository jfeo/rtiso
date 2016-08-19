#ifndef __CTW_RENDERER_H__
#define __CTW_RENDERER_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;
GLuint vertexBuffer;

/**
 * Initializes GLFW and GLEW and creates a GL context.
 */
void renderer_init();

/**
 * Swap buffers and poll events.
 */
void renderer_loop();

/**
 * Terminate GLFW.
 */
void renderer_deinit();

#endif // __CTW_RENDERER_H__
