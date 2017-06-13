/**
 * @file assembler.h
 * @brief Header file for assembler.c.
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../instruction.h"
#include "string_array.h"
#include "word_array.h"
#include "symbol_table.h"
#include "encode.h"
#include "../emulate_utils/print.h"
#include "../global.h"
#include "../toolbox.h"
#include "string_arrays.h"

mnemonic_t string_to_mnemonic(char *str);
opcode_t mnemonic_to_opcode(mnemonic_t mnemonic);
reg_address_t string_to_reg_address(char *str);
shift_t string_to_shift(char *str);

void parse_shift(string_array_t *tokens, instruction_t *instruction);
void parse_operand(string_array_t *tokens, instruction_t *instruction);
word_t parse_immediate_value(char *str);

word_t assemble_dpi(string_array_t *tokens);
word_t assemble_spl(string_array_t *tokens);
word_t assemble_mul(string_array_t *tokens);
word_t assemble_sdt(string_array_t *tokens, word_array_t *extra_words, int current_line, int max_lines);
word_t assemble_bra(string_array_t *tokens, symbol_table_t *symbol_table, address_t current_line);

void assemble_all_instructions(string_arrays_t *instructions, symbol_table_t *symbol_table, word_array_t *words);

#endif
