#include "string_array.h"
#include "symbol_table.h"
#include "string_array_array.h"

symbol_table_t *generate_symbol_table(string_array_array_t *tokens) {
  symbol_table_t *table = create_table(100);
  address_t address = 0;

  for (int i = 0; i < tokens->size; i++) {
    if (tokens->string_arrays[i]->size == 1) {
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
