#ifndef _RTISO_TEXT_H_
#define _RTISO_TEXT_H_

#include "coord.h"
#include "shader.h"

void text_init();
void text_update_projection(struct coord_window window_size);
struct text text_create();
void text_draw(const char *text, float x, float y);

#endif /* ifndef _RTISO_TEXT_H_ */
