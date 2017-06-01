#ifndef STRING_ARRAY_ARRAY_H
#define STRING_ARRAY_ARRAY_H
#include "string_array.h"
#include <stdlib.h>

typedef struct {
  uint16_t size;
  string_array_t *string_arrays[100];
} string_array_array_t;

#endif
