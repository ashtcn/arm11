#ifndef STRING_ARRAY_ARRAY_H
#define STRING_ARRAY_ARRAY_H
#include "string_array.h"
#include <stdlib.h>

typedef struct {
  uint16_t size;
  string_array_t *string_arrays[100];
} string_array_array_t;

string_array_array_t *make_string_array_array(void);
void add_string_array_array(string_array_array_t *arrays, string_array_t *array);

#endif
