#include <stdlib.h>
#include "global.h"
#include "assemble_toolbox.h"
#include "string_array_array.h"
#include "word_array.h"
#include "symbol_table.h"
#include "assembler.h"

string_array_array_t *tokenize_input(char **input, int input_lines) {
  string_array_array_t *result = make_string_array_array();

  for (int i = 0; i < input_lines; i++) {
    if(input[i][0] != '\n') {
    string_array_t *tokened = tokenize_instruction(input[i]);
    add_string_array_array(result, tokened);
    free(tokened->array);
    }
  }

  return result;
}

symbol_table_t *generate_symbol_table(string_array_array_t *tokens) {
  symbol_table_t *table = create_table(100);
  address_t address = 0;

  for (int i = 0; i < tokens->size; i++) {
    if (tokens->string_arrays[i]->size != 1) {
      address += 4;
    } else {
      for (int j = 0; tokens->string_arrays[i]->array[0][j]; j++) {
        if (tokens->string_arrays[i]->array[0][j] == ':') {
          tokens->string_arrays[i]->array[0][j] = '\0';
        }
      }
      add_row(table, tokens->string_arrays[i]->array[0], address);
      tokens->string_arrays[i]->array[0] = '\0';
    }
  }

  return table;
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
  return EXIT_SUCCESS;
}
