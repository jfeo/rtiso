#ifndef __RTISO_SHADER_H__
#define __RTISO_SHADER_H__

#include <GL/glew.h>

void shader_compile_debug(GLint shader);
void shader_link_debug(GLint program);

GLuint shader_create_from_file(GLenum type, const GLchar* file);
GLuint shader_create(GLenum type, const GLchar* src);
GLuint shader_program(GLuint vshader, GLuint fshader);
void shader_use_program(GLuint program);

#endif /* ifndef __RTISO_SHADER_H__ */
