#include "shader.h"

#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void shader_compile_debug(GLint shader)
{
  int msg_len = 0;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msg_len);

  if (msg_len > 0) {
    GLchar *msg = malloc(msg_len + 1);
    // error check for fail to allocate memory omitted
    glGetShaderInfoLog(shader, msg_len, NULL, msg);
    printf("%s", msg);
    free(msg);
  }
}

void shader_link_debug(GLint program) {
  GLint status;
  int msg_len = 0;

  glGetShaderiv(program, GL_INFO_LOG_LENGTH, &msg_len);

  if (msg_len > 0) {
    GLchar *msg = malloc(msg_len + 1);
    // error check for fail to allocate memory omitted
    glGetShaderInfoLog(program, msg_len, NULL, msg);
    printf("%s", msg);
    free(msg);
  }
}

GLuint shader_create(GLenum type, const GLchar* src) {
  GLint status;

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == 0)
  {
    printf("Error: Shader not compiled\n");
    shader_compile_debug(shader);
    return -1;
  }

  return shader;
}

GLuint shader_create_from_file(GLenum type, const GLchar* file) {
  GLchar* src = util_read_file(file);
  GLuint shader = shader_create(type, src);
  return shader;
}

GLuint shader_program(GLuint vshader, GLuint fshader) {
  GLint status;

  GLuint program = glCreateProgram();
  glAttachShader(program, vshader);
  glAttachShader(program, fshader);
  glLinkProgram(program);

  glDeleteShader(vshader);
  glDeleteShader(fshader);

  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if(status == 0) {
    printf("Error: Shader not linked\n");
    shader_link_debug(program);
    return -1;
  }

  return program;
}

void shader_use_program(GLuint program) {
  glUseProgram(program);
}
