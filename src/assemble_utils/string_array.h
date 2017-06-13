/**
 * @file string_array.h
 * @brief A header to define the string_array_t type.
 */

#ifndef STRING_ARRAY_H
#define STRING_ARRAY_H
#include <stdint.h>

/**
 * @brief A struct to hold an array of strings, and the size of that array.
 */
typedef struct {
  /** Array of strings. */
  char **array;
  /** The number of strings in the array. */
  uint16_t size;
} string_array_t;

#endif
