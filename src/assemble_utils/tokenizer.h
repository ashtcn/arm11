/**
 * @file tokenizer.h
 * @brief Header file for tokenizer.c.
 */

#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "assemble_toolbox.h"
#include "string_arrays.h"

string_arrays_t *tokenize_input(char **input, int input_lines);

#endif
