/**
 * @file symbol_table.h
 * @brief A header to define the symbol_table_t type.
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../global.h"
#include "string_arrays.h"

/**
 * @brief A row of a symbol table.
 */
typedef struct {
  /** The label string, with maximum length of MAX_LABEL_LENGTH */
  char label[MAX_LABEL_LENGTH + 1];
  /** The address of this label */
  address_t address;
} symbol_table_row_t;

/**
 * @brief A struct to represent a symbol table.
 */
typedef struct {
  /** Maximum number of rows in symbol table. */
  uint16_t max_size;
  /** Number of rows in symbol table. */
  uint16_t size;
  /** An array of symbol table rows. */
  symbol_table_row_t *rows;
} symbol_table_t;

bool is_label_in_table(symbol_table_t *table, char *label);
address_t get_address(symbol_table_t *table, char *label);
symbol_table_t *create_table(uint16_t row_number);
void add_row(symbol_table_t *table, char *label, address_t address);
void free_table(symbol_table_t *table);
symbol_table_t *generate_symbol_table(string_arrays_t *tokens);

#endif
