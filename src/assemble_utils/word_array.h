#ifndef WORD_ARRAY_H
#define WORD_ARRAY_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../global.h"

typedef struct {
  uint16_t max_elements;
  word_t *array;
  uint16_t size;
} word_array_t;

word_array_t *make_word_array(void);
void add_word_array(word_array_t *word_array, word_t word);
void free_word_array(word_array_t *word_array);

#endif
