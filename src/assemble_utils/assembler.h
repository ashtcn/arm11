/**
 * @file assembler.h
 * @brief Header file for assembler.c.
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "parser.h"

word_t assemble_dpi(string_array_t *tokens);
word_t assemble_spl(string_array_t *tokens);
word_t assemble_mul(string_array_t *tokens);
word_t assemble_sdt(string_array_t *tokens, word_array_t *extra_words,
                    int current_line, int max_lines);
word_t assemble_bra(string_array_t *tokens, symbol_table_t *symbol_table,
                    address_t current_line);

void assemble_all_instructions(string_arrays_t *instructions,
                               symbol_table_t *symbol_table,
                               word_array_t *words);

#endif
