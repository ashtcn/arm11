#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "instruction.h"
#include "string_array.h"
#include "symbol_table.h"

mnemonic_t string_to_mnemonic(char *str);
opcode_t mnemonic_to_opcode(mnemonic_t mnemonic);

word_t assemble_dpi(string_array_t *tokens, symbol_table_t *symbol_table);
word_t assemble_spl(string_array_t *tokens, symbol_table_t *symbol_table);
word_t assemble_mul(string_array_t *tokens, symbol_table_t *symbol_table);
word_t assemble_sdt(string_array_t *tokens, symbol_table_t *symbol_table);
word_t assemble_bra(string_array_t *tokens, symbol_table_t *symbol_table);

#endif
