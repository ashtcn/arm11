#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string_array.h"
#include "string_array_array.h"
#include "assemble_toolbox.h"
#include <stdbool.h>

void free_tokens(string_array_array_t *tokenized_input);

string_array_array_t *tokenize_input(char **input, int input_lines);
string_array_t *tokenize_operand_instruction(string_array_t *result, char* instruction_op, char* operands);
string_array_t *tokenize_instruction(char* instruction);

#endif
