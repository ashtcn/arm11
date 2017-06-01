#include <stdlib.h>
#include "global.h"
#include "assemble_toolbox.h"
#include "string_array_array.h"

  string_array_array_t *tokenize_input(char **input, int input_lines) {
  string_array_array_t *result = make_string_array_array();

  for (int i = 0; i < input_lines; i++) {
    add_string_array_array(result, tokenize_instruction(input[i]));
  }

  return result;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Incorrect number of arguments provided.\n");
    return EXIT_FAILURE;
  }

  char *load_filename = argv[1];
  char *save_filename = argv[2];

  int input_lines = lines_in_file(load_filename);
  char **loaded_file = load_source_file(load_filename, input_lines);
  string_array_array_t *tokenized_input = tokenize_input(loaded_file, input_lines);

  for (int i = 0; i < tokenized_input->size; i++) {
    for (int i2 = 0; i2 < tokenized_input->string_arrays[i]->size; i2++) {
    printf ("[%i][%i] %s\n", i, i2, tokenized_input->string_arrays[i]->array[i2]);
    }
  }


  word_t* output = {0};
  int output_file_size = 1;
  save_file(output, save_filename, output_file_size);

  free_2d_array(loaded_file);
  return EXIT_SUCCESS;
}
