#include "string_arrays.h"

string_arrays_t *make_string_arrays(void) {
  string_arrays_t *string_arrays = malloc(sizeof(string_arrays_t));
  if (!string_arrays) {
    perror("Unable to allocate memory for string arrays");
    exit(EXIT_FAILURE);
  }
  string_arrays->max_elements = INITIAL_ARRAY_SIZE;
  string_arrays->arrays = malloc(string_arrays->max_elements * sizeof(string_array_t *));
  if (!string_arrays->arrays) {
    perror("Unable to allocate memory for string arrays");
    exit(EXIT_FAILURE);
  }
  string_arrays->size = 0;
  return string_arrays;
}

void add_string_arrays(string_arrays_t *string_arrays, string_array_t *array) {
  if (string_arrays->size >= string_arrays->max_elements) {
    string_arrays->max_elements *= 2;
    string_arrays->arrays = realloc(string_arrays->arrays, string_arrays->max_elements * sizeof(string_array_t *));
    if (!string_arrays->arrays) {
      perror("Unable to expand size of string arrays");
      exit(EXIT_FAILURE);
    }
  }
  string_arrays->arrays[string_arrays->size] = array;
  string_arrays->size++;
}

void free_string_arrays(string_arrays_t *string_arrays) {
  for (int i = 0; i < string_arrays->size; i++) {
    for (int j = 1; j < string_arrays->arrays[i]->size; j++) {
      free(string_arrays->arrays[i]->array[j]);
    }
    free(string_arrays->arrays[i]->array);
    free(string_arrays->arrays[i]);
  }
  free(string_arrays->arrays);
  free(string_arrays);
}
