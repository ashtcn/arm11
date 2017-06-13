/**
 * @file string_arrays.h
 * @brief A header to define the string_arrays_t type.
 */

#ifndef string_arrays_H
#define string_arrays_H
#include "string_array.h"
#include "../global.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief A struct to hold an array of string arrays, and its size.
 */
typedef struct {
  /** The maximum number of string arrays that can be added to the array. */
  uint16_t max_elements;
  /** The array of string arrays. */
  string_array_t **arrays;
  /** The number of string arrays present. */
  uint16_t size;
} string_arrays_t;

string_arrays_t *make_string_arrays(void);
void add_string_arrays(string_arrays_t *arrays, string_array_t *array);
void free_string_arrays(string_arrays_t *arrays);

#endif
