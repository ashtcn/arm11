/**
 * @file assemble.c
 * @brief The main functionality for the ARM11 assembler.
 */

#include <stdlib.h>
#include "global.h"
#include "assemble_utils/assemble_toolbox.h"
#include "assemble_utils/tokenizer.h"
#include "assemble_utils/string_arrays.h"
#include "assemble_utils/word_array.h"
#include "assemble_utils/symbol_table.h"
#include "assemble_utils/assembler.h"

/**
 * @brief Assembles ARM assembly into ARM binary object code.
 *
 * The user must provide two arguments, which are a valid file name for a file
 * containing ARM11 assembly instructions, and a file location to which the
 * resulting ARM11 binary object code file will be written.
 */
int main(int argc, char **argv) {
  // Check arguments
  if (argc != 3) {
    fprintf(stderr, "Incorrect number of arguments provided.\n");
    return EXIT_FAILURE;
  }

  char *load_filename = argv[1];
  char *save_filename = argv[2];

  // Load the assembly instructions and tokenizes, generates symbol table
  int input_lines = lines_in_file(load_filename);
  char **loaded_file = load_source_file(load_filename, input_lines);
  string_arrays_t *tokenized_input = tokenize_input(loaded_file, input_lines);

  symbol_table_t *s = generate_symbol_table(tokenized_input);

  // Assembles into binary object code, saves to given file
  word_array_t *output_data = make_word_array();
  assemble_all_instructions(tokenized_input, s, output_data);

  save_file(output_data->array, save_filename, output_data->size);

  // Free allocated memory and exit
  free_word_array(output_data);
  free_string_arrays(tokenized_input);
  free_2d_array(loaded_file, input_lines);
  free_table(s);
  return EXIT_SUCCESS;
}
