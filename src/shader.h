#ifndef __RTISO_SHADER_H__
#define __RTISO_SHADER_H__

#include <GL/gl.h>

#include "array.h"

extern struct shader_program world_shader;

struct shader_attr {
  const char *name;
  GLuint location;
  GLuint size;
  GLenum type;
  GLboolean normalized;
  GLuint stride;
  GLvoid *pointer;
  GLuint divisor;
};

ARRAY_DECLARE(struct shader_attr, shader_attr)

struct shader_uniform {
  const char *name;
  GLuint location;
};

ARRAY_DECLARE(struct shader_uniform, shader_uniform)

struct shader_program {
  GLuint program_id;
  const GLchar *name;
  array_shader_attr attrs;
  array_shader_uniform unis;
};

void shader_compile_debug(GLint shader);
void shader_link_debug(GLint program);

GLuint shader_create_from_file(GLenum type, const GLchar *file);
GLuint shader_create(GLenum type, const GLchar *src);
GLuint shader_program(GLuint vshader, GLuint fshader);
void shader_use_program(GLuint program);

struct shader_program shader_program_from_file(const GLchar *vertex_src,
                                               const GLchar *frag_src,
                                               const GLchar *name);
void shader_program_add_attr(struct shader_program *shader,
                             struct shader_attr attr);
void shader_program_enable_attr(struct shader_program *shader,
                                const GLchar *name);
void shader_program_add_uniform(struct shader_program *shader,
                                struct shader_uniform unif);
void shader_program_use(struct shader_program *shader);

struct shader_find_uni {
  GLboolean found;
  struct shader_uniform uni;
};

struct shader_find_uni shader_find_uni(struct shader_program *shader,
                                       const GLchar *name);

void shader_uniform_mat4(struct shader_uniform *unif, GLsizei count,
                         GLboolean transpose, GLfloat *value);

void shader_program_uniform_mat4(struct shader_program *shader,
                                 const GLchar *name, GLsizei count,
                                 GLboolean transpose, GLfloat *value);

void shader_uniform_vec3(struct shader_uniform *unif, GLsizei count,
                         GLfloat *value);

void shader_program_uniform_vec3(struct shader_program *shader,
                                 const GLchar *name, GLsizei count,
                                 GLfloat *value);
#endif /* ifndef __RTISO_SHADER_H__ */
