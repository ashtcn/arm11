/**
 * @file word_array.h
 * @brief A header to define the word_array_t type.
 */

#ifndef WORD_ARRAY_H
#define WORD_ARRAY_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../global.h"

/**
 * @brief A struct to hold an array of words, and the size of that array.
 */
typedef struct {
  /** The maximum number of words that can be added to the array. */
  uint16_t max_elements;
  /** The array of words. */
  word_t *array;
  /** The number of words present. */
  uint16_t size;
} word_array_t;

word_array_t *make_word_array(void);
void add_word_array(word_array_t *word_array, word_t word);
void free_word_array(word_array_t *word_array);

#endif
