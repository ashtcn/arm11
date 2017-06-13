/**
 * @file symbol_table.c
 * @brief File for functions for using and making symbol tables.
 */

#include "symbol_table.h"

/**
 * @brief Creates and initialises a symbol table.
 *
 * @param row_number Initial maximum size of the table.
 * @returns An initialised symbol table.
 */
symbol_table_t *create_table(uint16_t row_number) {
  symbol_table_t *table = malloc(sizeof(symbol_table_t));
  if (!table) {
    perror("Unable to allocate memory for symbol table");
    exit(EXIT_FAILURE);
  }
  table->rows = malloc(sizeof(symbol_table_row_t) * row_number);
  if (!table->rows) {
    perror("Unable to allocate memory for symbol table");
    exit(EXIT_FAILURE);
  }
  table->max_size = row_number;
  table->size = 0;
  return table;
}

/**
 * @brief Adds a row to a symbol table.
 *
 * Given a label and a address, will allocate memory for the row, and add it
 * to the table. It will dynamically increase the array size if more labels are
 * added.
 *
 * @param table The table to add the row to.
 * @param label The string of the label.
 * @param address The address of the label.
 */
void add_row(symbol_table_t *table, char *label, address_t address) {
  if (is_label_in_table(table, label)) {
    fprintf(stderr, "Label already in symbol table");
    exit(EXIT_FAILURE);
  }
  symbol_table_row_t row = {
    .address = address,
  };
  memcpy(row.label, label, MAX_LABEL_LENGTH + 1);
  if (table->size >= table->max_size) {
    table->max_size *= 2;
    table->rows = realloc(table->rows, table->max_size
                                       * sizeof(symbol_table_row_t));
    if (!table->rows) {
      perror("Unable to expand size of symbol table");
      exit(EXIT_FAILURE);
    }
  }
  table->rows[table->size] = row;
  ++table->size;
}

/**
 * @brief Gets the address of a label in a symbol table.
 *
 * If the label is not in the table it will print an error and stop the program.
 *
 * @param table The table which the label is in.
 * @param label The string of the label.
 * @returns The address that corresponds to the label.
 */
address_t get_address(symbol_table_t *table, char *label) {
  for (uint32_t i = 0; i < table->size; ++i) {
    if (!strcmp(table->rows[i].label, label)) {
      return table->rows[i].address;
    }
  }
  fprintf(stderr, "Label not in symbol table");
  exit(EXIT_FAILURE);
}

/**
 * @brief Checks if the label is in the table.
 *
 * @param table The table to check if the label is in.
 * @param label The string of the label.
 * @returns True if the label is in the table, false otherwise.
 */
bool is_label_in_table(symbol_table_t *table, char *label) {
  for (uint32_t i = 0; i < table->size; ++i) {
    if (!strcmp(table->rows[i].label, label)) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Generates a symbol table given a block of assembly.
 *
 * @param tokens The assembly, tokenised.
 * @returns The complete symbol table.
 */
symbol_table_t *generate_symbol_table(string_arrays_t *tokens) {
  symbol_table_t *table = create_table(16);
  address_t address = 0;

  for (int i = 0; i < tokens->size; i++) {
    if (tokens->arrays[i]->size != 1) {
      address += 4;
    } else {
      for (int j = 0; tokens->arrays[i]->array[0][j]; j++) {
        if (tokens->arrays[i]->array[0][j] == ':') {
          tokens->arrays[i]->array[0][j] = '\0';
        }
      }
      add_row(table, tokens->arrays[i]->array[0], address);
      tokens->arrays[i]->array[0] = '\0';
    }
  }

  return table;
}

/**
 * @brief Frees all memory used by a symbol table.
 *
 * @param table The table to free.
 */
void free_table(symbol_table_t *table) {
  free(table->rows);
  free(table);
}
