/**
 * @file assemble_toolbox.c
 * @brief Miscellaneous functions for assemble.
 */

#include "assemble_toolbox.h"

/**
 * @brief Allocates the memory for a 2D array of chars.
 *
 * @param rows Rows of the 2D array.
 * @param cols Columns of the 2D array.
 * @returns The pointer to the first element of the 2D array.
 */
char **create_2d_array(unsigned int rows, unsigned int cols) {
  unsigned int i;
  char **loaded_file = malloc(rows * sizeof(char *));
  if (!loaded_file) {
    perror("Error allocating memory for source file");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < rows; i++) {
    loaded_file[i] = malloc(cols * sizeof(char));
    if (!loaded_file[i]) {
      perror("Error allocating memory for source file");
      exit(EXIT_FAILURE);
    }
  }
  return loaded_file;
}

/**
 * @brief Frees all memory used for the 2D array of chars
 *
 * @param arr The 2D array to free.
 * @param rows Number of rows of the 2D array.
 */
void free_2d_array(char **arr, int rows) {
  for (int i = 0; i < rows; i++) {
    free(arr[i]);
  }
  free(arr);
}

/**
 * @brief Returns the number of lines in a file.
 *
 * @param file_name Address of the file to read from.
 * @returns Number of lines in a file.
 */
int lines_in_file(char *file_name) {
  int lines = 0;
  char ch;
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    perror("Error in opening source file");
    exit(EXIT_FAILURE);
  }
  bool on_new_line = true;
  while ((ch = getc(file)) != EOF) {
    if (ch == '\n') {
      if (!on_new_line) {
        lines++;
      }
      on_new_line = true;
    } else {
      on_new_line = false;
    }
  }
  fclose(file);
  return lines;
}

/**
 * @brief Returns a 2D array of the given source file.
 *
 * The 2D array is constructed such that is an array of strings, where each
 * string is a line from the file.
 *
 * @param load_filename Address of the file to load from.
 * @param lines Number of lines in the file.
 * @returns A 2D array made from the file.
 */
char **load_source_file(char *load_filename, int lines) {
  FILE *file = fopen(load_filename, "r");
  if (file == NULL) {
    perror("Error in opening source file");
    exit(EXIT_FAILURE);
  }

  //Set up the correct size array;
  char **loaded_file = create_2d_array(lines, MAX_LINE_LENGTH);

  // Try to read the file line by line
  int size = 0;
  while (size < lines && fgets(loaded_file[size], MAX_LINE_LENGTH, file)) {
    // Strips any trailing newlinesword_size
    if (loaded_file[size][0] != '\n' && loaded_file[size][0] != '\r') {
      loaded_file[size][strcspn(loaded_file[size], "\n")] = 0;
      size++;
    }
	}
  fclose(file);
  return loaded_file;
}

/**
 * @brief Saves an array of words to a file.
 *
 * @param data Array of words to write to the file.
 * @param file_name Address of the file to save to.
 * @param file_size Size of the word array.
 */
void save_file(word_t *data, char *file_name, int file_size) {
  FILE *file = fopen(file_name, "wb");
  if (file == NULL) {
    perror("Error in opening save file");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < file_size; i++) {
    fwrite(&data[i], sizeof(word_t), 1, file);
  }

  fclose(file);
}
