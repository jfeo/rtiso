#ifndef _RTISO_TIMER_H_
#define _RTISO_TIMER_H_

#include <sys/time.h>

struct timer {
  struct timeval tv_prev;
};

struct timer timer_init();
double timer_elapsed_ms(struct timer *timer);
void msleep(int ms);

#endif /* ifndef _RTISO_TIMER_H_ */
