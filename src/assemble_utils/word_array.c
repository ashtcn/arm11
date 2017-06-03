#include "word_array.h"

word_array_t *make_word_array(void) {
  word_array_t *array = malloc(sizeof(word_array_t));
  array->size = 0;
  return array;
}

void add_word_array(word_array_t *word_array, word_t word) {
  word_array->array[word_array->size] = word;
  word_array->size++;
}
