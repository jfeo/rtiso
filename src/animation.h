#ifndef __RTISO_ANIMATION_H__
#define __RTISO_ANIMATION_H__

struct animation {
  int frames;
  int current_frame;
  int frame_width;
  float rate; // ms per frame
  float overflow;
  struct texture *tex;
};

struct animation *animation_create(struct texture *tex, int frames, float rate,
                                   int frame_width);

void animation_update(struct animation *anim, float ms);

#endif /* ifndef __RTISO_ANIMATION_H__ */
