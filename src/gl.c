#include "GL/gl.h"
#include <stdarg.h>
#include <stdio.h>

GLint gl_check_error(const char *fmt, ...) {
  GLint err = glGetError();
  if (err != 0) {
    char msg[64];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(msg, 64, fmt, ap);
    va_end(ap);

    printf("Error '%d': %s\n", err, msg);
  }

  return err;
}
