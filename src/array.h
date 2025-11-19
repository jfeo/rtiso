#ifndef _RTISO_ARRAY_H_
#define _RTISO_ARRAY_H_

#include <stdlib.h>

#define ARRAY_INITIAL_ALLOCATED 10

#define ARRAY_DECLARE(t, tname)                                                \
  typedef struct {                                                             \
    t *elems;                                                                  \
    unsigned int count;                                                        \
    unsigned int allocated;                                                    \
  } array_##tname;                                                             \
  array_##tname array_##tname##_init();                                        \
  void array_##tname##_add(array_##tname *arr, t elem);

#define ARRAY_DEFINE(t, tname)                                                 \
  array_##tname array_##tname##_init() {                                       \
    array_##tname a;                                                           \
    a.elems = (t *)malloc((sizeof(t)) * ARRAY_INITIAL_ALLOCATED);              \
    a.allocated = ARRAY_INITIAL_ALLOCATED;                                     \
    a.count = 0;                                                               \
    return a;                                                                  \
  }                                                                            \
  void array_##tname##_add(array_##tname *arr, t elem) {                       \
    if (arr->count == arr->allocated) {                                        \
      arr->allocated *= 2;                                                     \
      arr->elems = (t *)realloc(arr->elems, (sizeof(t)) * arr->allocated);     \
    }                                                                          \
    arr->elems[arr->count] = elem;                                             \
    arr->count++;                                                              \
  }

#endif /* ifndef _RTISO_ARRAY_H_ */
