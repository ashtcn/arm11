/**
 * @file assembler.h
 * @brief Header file for assembler.c.
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "parser.h"

void assemble_all_instructions(string_arrays_t *instructions,
                               symbol_table_t *symbol_table,
                               word_array_t *words);

#endif
