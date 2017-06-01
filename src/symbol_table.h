#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdlib.h>
#include "global.h"

typedef struct {
  char *label;
  address_t address;
} symbol_table_row_t;

typedef struct {
  // Maximum number of rows in symbol table
  uint16_t max_size;

  // Number of rows in symbol table
  uint16_t size;
  
  symbol_table_row_t *rows;
} symbol_table_t;

#endif
