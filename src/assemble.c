#include <stdlib.h>
#include "global.h"
#include "assemble_toolbox.h"

void t_test(char *test) {
  string_array_t *tokens = tokenize_instruction(test);
  for (int it = 0; it < tokens->size; it++) {
    printf ("tokens [%i] %s\n", it, tokens->array[it]);
  }
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

  for (int it = 0; it < input_lines; it++) {
    printf ("[%i] %s\n", it, loaded_file[it]);
    t_test(loaded_file[it]);
  }


  word_t* output = {0};
  int output_file_size = 1;
  save_file(output, save_filename, output_file_size);

  free_2d_array(loaded_file);
  return EXIT_SUCCESS;
}
