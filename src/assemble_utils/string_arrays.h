#ifndef string_arrays_H
#define string_arrays_H
#include "string_array.h"
#include "../global.h"
#include <stdlib.h>

typedef struct {
  uint16_t max_elements;
  string_array_t **arrays;
  uint16_t size;
} string_arrays_t;

string_arrays_t *make_string_arrays(void);
void add_string_arrays(string_arrays_t *arrays, string_array_t *array);
void free_string_arrays(string_arrays_t *arrays);

#endif
