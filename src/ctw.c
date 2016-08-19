/*
 * file: main.c
 * author: jfeo (jens@feodor.dk)
 * date: 17/08/2016
 */

#include <stdlib.h>
#include "renderer.h"

int main(int argc, char *argv[])
{

  renderer_init();
  renderer_loop();
  renderer_deinit();

  return EXIT_SUCCESS;;
}
