#include <stdlib.h>
#include "global.h"
#include "assemble_toolbox.h"
#include "string_array_array.h"
#include "symbol_table.h"
#include "assembler.h"

  string_array_array_t *tokenize_input(char **input, int input_lines) {
  string_array_array_t *result = make_string_array_array();

  for (int i = 0; i < input_lines; i++) {
    add_string_array_array(result, tokenize_instruction(input[i]));
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

  for (int i = 0; i < tokenized_input->size; i++) {
    for (int i2 = 0; i2 < tokenized_input->string_arrays[i]->size; i2++) {
      printf ("[%i][%i] %s\n", i, i2, tokenized_input->string_arrays[i]->array[i2]);
    }
    printf ("[%i] %08x\n", i, assemble_dpi(tokenized_input->string_arrays[i]));
  }

  symbol_table_t *s = generate_symbol_table(tokenized_input);
  for (int i = 0; i < s->size; i++) {
    printf ("[%i] address:%u  label:%s\n", i, s->rows[i].address, s->rows[i].label);
  }


  int output_file_size = tokenized_input->size;
  word_t output_data[output_file_size];
  for (int i = 0; i < tokenized_input->size; i++) {
    output_data[i] = assemble_dpi(tokenized_input->string_arrays[i]);
  }

  save_file(output_data, save_filename, output_file_size);

  for (int i = 0; i < tokenized_input->size; i++) {
    free(tokenized_input->string_arrays[i]->array);
    free(tokenized_input->string_arrays[i]);
  }
  free(tokenized_input);
  free_2d_array(loaded_file);
  return EXIT_SUCCESS;
}
