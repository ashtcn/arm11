/**
 * @file word_array.c
 * @brief Functions for using word arrays.
 */
#include "word_array.h"

/**
 * @brief Initialises a word array
 *
 * Mallocs the memory for the array, exits the program if that was not possible.
 * It initialises the size of the array to INITIAL_ARRAY_SIZE, a #define in
 * global.h.
 * @return An initinalised word array.
 */
word_array_t *make_word_array(void) {
  word_array_t *word_array = malloc(sizeof(word_array_t));
  if (!word_array) {
    perror("Unable to allocate memory for word array");
    exit(EXIT_FAILURE);
  }
  word_array->max_elements = INITIAL_ARRAY_SIZE;
  word_array->array = malloc(word_array->max_elements * sizeof(word_t));
  if (!word_array->array) {
    perror("Unable to allocate memory for word array");
    exit(EXIT_FAILURE);
  }
  word_array->size = 0;
  return word_array;
}

/**
 * @brief Adds a word to a word array
 *
 * If more memory is required it doubles the array size.
 * @param word_array The word array to add the word to.
 * @param word The word to add to the array.
 */
void add_word_array(word_array_t *word_array, word_t word) {
  if (word_array->size >= word_array->max_elements) {
    word_array->max_elements *= 2;
    word_array->array = realloc(word_array->array, word_array->max_elements * sizeof(word_t));
    if (!word_array->array) {
      perror("Unable to dynamically increase size of word array");
      exit(EXIT_FAILURE);
    }
  }
  word_array->array[word_array->size] = word;
  word_array->size++;
}

/**
 * @brief Frees all memory used in a word array.
 *
 * @param word_array The word array to free.
 */
void free_word_array(word_array_t *word_array) {
  free(word_array->array);
  free(word_array);
}
