#define _POSIX_C_SOURCE 199309L

#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "timer.h"

struct timer timer_init() {
  struct timer t;

  gettimeofday(&t.tv_prev, NULL);

  return t;
}

float timer_elapsed_ms(struct timer *timer) {
  struct timeval tv_now;

  gettimeofday(&tv_now, NULL);
  float ms = (tv_now.tv_sec - timer->tv_prev.tv_sec) * 1000.0f +
             (tv_now.tv_usec - timer->tv_prev.tv_usec) / 1000.0f;
  timer->tv_prev = tv_now;

  return ms;
}

void msleep(int msec) {
  struct timespec ts;
  int res;

  if (msec < 0) {
    errno = EINVAL;
    return;
  }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);

  return;
}
