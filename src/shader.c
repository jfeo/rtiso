#include "GL/glew.h"

#include "shader.h"

#include "GL/gl.h"

#include "array.h"
#include "gl.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ARRAY_DEFINE(struct shader_attr, shader_attr)

ARRAY_DEFINE(struct shader_uniform, shader_uniform)

void shader_compile_debug(GLint shader) {
  int msg_len = 0;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msg_len);

  if (msg_len > 0) {
    GLchar *msg = malloc(msg_len + 1);
    // error check for fail to allocate memory omitted
    glGetShaderInfoLog(shader, msg_len, NULL, msg);
    printf("Error shader: %s", msg);
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

GLuint shader_create(GLenum type, const GLchar *src) {
  GLint status;

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == 0) {
    printf("Error: Shader not compiled\n");
    shader_compile_debug(shader);
    return -1;
  }

  return shader;
}

GLuint shader_create_from_file(GLenum type, const GLchar *file) {
  GLchar *src = util_read_file(file);
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
  if (status == 0) {
    printf("Error: Shader not linked\n");
    shader_link_debug(program);
    return -1;
  }

  return program;
}

void shader_use_program(GLuint program) { glUseProgram(program); }

struct shader_program shader_program_from_file(const GLchar *vertex_src,
                                               const GLchar *frag_src,
                                               const GLchar *name) {
  struct shader_program shader = {.name = name};

  GLuint vshader = shader_create_from_file(GL_VERTEX_SHADER, vertex_src);
  gl_check_error("create and compile vertex shader from file '%s'", vertex_src);

  GLuint fshader = shader_create_from_file(GL_FRAGMENT_SHADER, frag_src);
  gl_check_error("create and compile fragment shader from file '%s'", frag_src);

  shader.program_id = shader_program(vshader, fshader);
  gl_check_error("link shader program '%s'", name);

  shader.attrs = array_shader_attr_init();
  shader.unis = array_shader_uniform_init();

  return shader;
}

void shader_program_add_attr(struct shader_program *shader,
                             struct shader_attr attr) {
  printf(" shader '%s' adding attrib '%s' with divisor=%d\n", shader->name,
         attr.name, attr.divisor);
  attr.location = glGetAttribLocation(shader->program_id, attr.name);

  array_shader_attr_add(&shader->attrs, attr);
}

struct shader_find_attr {
  GLboolean found;
  struct shader_attr attr;
};

struct shader_find_attr shader_find_attr(struct shader_program *shader,
                                         const GLchar *name) {
  struct shader_find_attr res = {0};

  for (int i = 0; i < shader->attrs.count; i++) {
    if (strcmp(shader->attrs.elems[i].name, name) == 0) {
      res.attr = shader->attrs.elems[i];
      res.found = GL_TRUE;
      break;
    }
  }

  return res;
}

void shader_program_enable_attr(struct shader_program *shader,
                                const GLchar *name) {
  struct shader_attr attr;
  struct shader_find_attr find_attr = shader_find_attr(shader, name);
  if (find_attr.found) {
    attr = find_attr.attr;
  } else {
    return;
  }

  switch (attr.type) {
  case GL_FLOAT:
    glVertexAttribPointer(attr.location, attr.size, attr.type, attr.normalized,
                          attr.stride, attr.pointer);
    break;
  case GL_INT:
    glVertexAttribIPointer(attr.location, attr.size, attr.type, attr.stride,
                           attr.pointer);
    break;
  default:
    printf("Error: cannot add invalid attribute type %u\n",
           find_attr.attr.type);
  }
  gl_check_error("add attribute");

  glEnableVertexAttribArray(find_attr.attr.location);
  gl_check_error("enable attribute");

  glVertexAttribDivisor(find_attr.attr.location, find_attr.attr.divisor);
  gl_check_error("set vertex divisor");
}

void shader_program_add_uniform(struct shader_program *shader,
                                struct shader_uniform uni) {
  uni.location = glGetUniformLocation(shader->program_id, uni.name);
  gl_check_error("get uniform location");

  array_shader_uniform_add(&shader->unis, uni);
}

struct shader_find_uni shader_find_uni(struct shader_program *shader,
                                       const GLchar *name) {
  struct shader_find_uni res = {0};

  for (int i = 0; i < shader->unis.count; i++) {
    if (strcmp(shader->unis.elems[i].name, name) == 0) {
      res.uni = shader->unis.elems[i];
      res.found = GL_TRUE;
      break;
    }
  }

  return res;
}

void shader_uniform_mat4(struct shader_uniform *unif, GLsizei count,
                         GLboolean transpose, GLfloat *value) {
  glUniformMatrix4fv(unif->location, count, transpose, value);
}

void shader_program_uniform_mat4(struct shader_program *shader,
                                 const GLchar *name, GLsizei count,
                                 GLboolean transpose, GLfloat *value) {
  struct shader_find_uni find_uni = shader_find_uni(shader, name);

  if (find_uni.found == GL_FALSE) {
    printf("Error: uniform %s not found\n", name);
    return;
  }

  shader_uniform_mat4(&find_uni.uni, count, transpose, value);
  gl_check_error("uniform mat4 name=%s location=%d", find_uni.uni.name,
                 find_uni.uni.location);
}

void shader_uniform_vec3(struct shader_uniform *unif, GLsizei count,
                         GLfloat *value) {
  glUniform3fv(unif->location, count, value);
}

void shader_program_uniform_vec3(struct shader_program *shader,
                                 const GLchar *name, GLsizei count,
                                 GLfloat *value) {
  struct shader_find_uni find_uni = shader_find_uni(shader, name);

  if (!find_uni.found) {
    printf("Error: uniform %s not found\n", name);
    return;
  }

  shader_uniform_vec3(&find_uni.uni, count, value);
  gl_check_error("uniform vec3");
}

void shader_program_use(struct shader_program *shader) {
  glUseProgram(shader->program_id);
}
