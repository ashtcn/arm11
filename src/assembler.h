#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "instruction.h"
#include "string_array.h"
#include "word_array.h"
#include "symbol_table.h"
#include "encode.h"
#include "print.h"
#include "string_array_array.h"

mnemonic_t string_to_mnemonic(char *str);
opcode_t mnemonic_to_opcode(mnemonic_t mnemonic);
reg_address_t string_to_reg_address(char *str);
shift_t string_to_shift(char *str);

void parse_shift(string_array_t *tokens, instruction_t *instruction);
void parse_operand(string_array_t *tokens, instruction_t *instruction);

word_t assemble_dpi(string_array_t *tokens);
word_t assemble_spl(string_array_t *tokens);
word_t assemble_mul(string_array_t *tokens);
word_t assemble_sdt(string_array_t *tokens);
word_t assemble_bra(string_array_t *tokens, symbol_table_t *symbol_table);

void assemble_all_instructions(string_array_array_t *instructions, symbol_table_t *symbol_table, word_array_t *words);
word_t assemble_instruction(string_array_t *tokens, symbol_table_t *symbol_table, word_array_t *extra_words, int current_line_number, int max_lines);

#endif
