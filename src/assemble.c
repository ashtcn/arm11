#include <stdlib.h>
#include "global.h"
#include "assemble_utils/assemble_toolbox.h"
#include "assemble_utils/string_array_array.h"
#include "assemble_utils/word_array.h"
#include "assemble_utils/symbol_table.h"
#include "assemble_utils/assembler.h"
#include "assemble_utils/symbol_table.h"

string_array_array_t *tokenize_input(char **input, int input_lines) {
  string_array_array_t *result = make_string_array_array();

  for (int i = 0; i < input_lines; i++) {
    if(input[i][0] != '\n') {
    string_array_t *tokened = tokenize_instruction(input[i]);
    add_string_array_array(result, tokened);
    }
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

  symbol_table_t *s = generate_symbol_table(tokenized_input);

  word_array_t *output_data = make_word_array();
  assemble_all_instructions(tokenized_input, s, output_data);

  save_file(output_data->array, save_filename, output_data->size);

  free(output_data);
  for (int i = 0; i < tokenized_input->size; i++) {
    free(tokenized_input->string_arrays[i]->array);
    free(tokenized_input->string_arrays[i]);
  }
  free(tokenized_input);
  free_2d_array(loaded_file);
  free_table(s);
  return EXIT_SUCCESS;
}
