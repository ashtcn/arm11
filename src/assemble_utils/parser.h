/**
 * @file parser.h
 * @brief Header file for parser.c.
 */

#ifndef PARSER_H
#define PARSER_H
#include "../toolbox.h"
#include "encode.h"
#include "symbol_table.h"
#include "word_array.h"

mnemonic_t string_to_mnemonic(char *str);
condition_t string_to_condition(char *str);
opcode_t mnemonic_to_opcode(mnemonic_t mnemonic);
reg_address_t string_to_reg_address(char *str);
shift_t string_to_shift(char *str);

void parse_shift(string_array_t *tokens, instruction_t *instruction);
void parse_operand(string_array_t *tokens, instruction_t *instruction);

word_t parse_immediate_value(char *str);

#endif
