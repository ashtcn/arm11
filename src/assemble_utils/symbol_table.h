#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../global.h"
#include "string_array_array.h"

typedef struct {
  char label[15];
  address_t address;
} symbol_table_row_t;

typedef struct {
  // Maximum number of rows in symbol table
  uint16_t max_size;

  // Number of rows in symbol table
  uint16_t size;

  symbol_table_row_t *rows;
} symbol_table_t;

bool is_label_in_table(symbol_table_t *table, char *label);
address_t get_address(symbol_table_t *table, char *label);
symbol_table_t *create_table(uint16_t row_number);
void add_row(symbol_table_t *table, char *label, address_t address);
void free_table(symbol_table_t *table);
symbol_table_t *generate_symbol_table(string_array_array_t *tokens);

#endif
