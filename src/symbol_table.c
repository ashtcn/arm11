#include "symbol_table.h"

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

void add_row(symbol_table_t *table, char *label, address_t address) {
  if (is_label_in_table(table, label)) {
    fprintf(stderr, "Label already in symbol table");
  }
  symbol_table_row_t row = {
    .address = address,
  };
  memcpy(row.label, label, 15);
  table->rows[table->size] = row;
  ++table->size;
}

address_t get_address(symbol_table_t *table, char *label) {
  // PRE: Label is in the symbol table
  for (uint32_t i = 0; i < table->size; ++i) {
    if (!strcmp(table->rows[i].label, label)) {
      return table->rows[i].address;
    }
  }
  fprintf(stderr, "Label not in symbol table");
  return 0;
}

bool is_label_in_table(symbol_table_t *table, char *label) {
  for (uint32_t i = 0; i < table->size; ++i) {
    if (!strcmp(table->rows[i].label, label)) {
      return true;
    }
  }
  return false;
}
