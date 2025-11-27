#include "timer.h"
#include "unistd.h"

struct timer timer_init() {
  struct timer t;

  gettimeofday(&t.tv_prev, NULL);

  return t;
}

double timer_elapsed_ms(struct timer *timer) {
  struct timeval tv_now;

  gettimeofday(&tv_now, NULL);
  double ms = (tv_now.tv_sec - timer->tv_prev.tv_sec) * 1000.0f +
              (tv_now.tv_usec - timer->tv_prev.tv_usec) / 1000.0f;
  timer->tv_prev = tv_now;

  return ms;
}
