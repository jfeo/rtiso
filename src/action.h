#ifndef __RTISO_ACTION_H__
#define __RTISO_ACTION_H__

#include "coord.h"

enum ACTION_TYPE { ACTION_NONE, ACTION_MOVE };

struct action_move {
  struct coord_real to;
};

struct action {
  enum ACTION_TYPE type;
  union {
    struct action_move move;
  } data;
};

void action_update(struct action *a);

#endif /* ifndef __RTISO_ACTION_H__ */
