#ifndef STRING_ARRAY_H
#define STRING_ARRAY_H
#include <stdint.h>

typedef struct {
  char **array;
  uint16_t size;
} string_array_t;

#endif
