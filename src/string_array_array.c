#include "string_array_array.h"

string_array_array_t *make_string_array_array(void) {
  string_array_array_t *array = malloc(sizeof(string_array_array_t));
  array->size = 0;
  return array;
}

void add_string_array_array(string_array_array_t *arrays, string_array_t *array) {
  arrays->string_arrays[arrays->size] = array;
  arrays->size++;
}
