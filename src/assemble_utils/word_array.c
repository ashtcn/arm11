#include "word_array.h"

word_array_t *make_word_array(void) {
  word_array_t *word_array = malloc(sizeof(word_array_t));
  word_array->max_elements = INITIAL_ARRAY_SIZE;
  word_array->array = malloc(word_array->max_elements * sizeof(word_t));
  word_array->size = 0;
  return word_array;
}

void add_word_array(word_array_t *word_array, word_t word) {
  if (word_array->size >= word_array->max_elements) {
    word_array->max_elements *= 2;
    word_array->array = realloc(word_array->array, word_array->max_elements * sizeof(word_t));
  }
  word_array->array[word_array->size] = word;
  word_array->size++;
}

void free_word_array(word_array_t *word_array) {
  free(word_array->array);
  free(word_array);
}
