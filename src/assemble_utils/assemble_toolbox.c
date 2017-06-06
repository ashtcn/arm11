#include "assemble_toolbox.h"

void save_file(word_t *data, char *file_name, int file_size) {
  FILE *file = fopen(file_name, "wb");
  if (file == NULL) {
    perror("Error in opening save file.");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < file_size; i++) {
    fwrite(&data[i], sizeof(word_t), 1, file);
  }

  fclose(file);
}

int lines_in_file(char *file_name) {
  int lines = 0;
  char ch;
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    perror("Error in opening source file.");
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

char **create_2d_array(unsigned int rows, unsigned int cols) {
  unsigned int i;
  char **loaded_file = malloc(rows * sizeof(char *));
  if (!loaded_file) {
    perror("Error allocating memory for source file.");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < rows; i++) {
    loaded_file[i] = malloc(cols * sizeof(char));
  }
  return loaded_file;
}

void free_2d_array(char **arr, int rows) {
  for (int i = 0; i < rows; i++) {
    free(arr[i]);
  }
  free(arr);
}

char **load_source_file(char *load_filename, int lines) {
  int max_line_length = 512; //put in global
  FILE *file = fopen(load_filename, "r");
  if (file == NULL) {
    perror("Error in opening source file.");
    exit(EXIT_FAILURE);
  }

  //Set up the correct size array;
  char **loaded_file = create_2d_array(lines, max_line_length);

  // Try to read the file line by line
  int size = 0;
  while (size < lines && fgets(loaded_file[size], max_line_length, file)) {
    // Strips any trailing newlinesword_size
    if (loaded_file[size][0] != '\n' && loaded_file[size][0] != '\r') {
      loaded_file[size][strcspn(loaded_file[size], "\n")] = 0;
      size++;
    }
	}
  fclose(file);
  return loaded_file;
}
