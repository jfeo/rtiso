#include <stdlib.h>
#include "animation.h"

struct animation* animation_create(struct texture* tex, int frames, float rate, int frame_width) {
	struct animation* anim = (struct animation*)malloc(sizeof(struct animation));
	anim->tex = tex;
	anim->frames = frames;
	anim->frame_width = frame_width;
	anim->rate = rate;
	anim->overflow = 0.0f;
	return anim;
}

void animation_update(struct animation* anim, float ms) {
	anim->overflow += ms;
	if (anim->overflow >= anim->rate) {
		anim->overflow -= anim->rate;
		anim->current_frame = (anim->current_frame + 1) % anim->frames;
	}
}
