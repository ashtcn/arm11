/**
 * @file encode.h
 * @brief Header file for encode.c.
 */

#ifndef ENCODE_H
#define ENCODE_H
#include <stdio.h>
#include "../instruction.h"

word_t encode(instruction_t *instruction);
word_t add_cond(instruction_t *instruction);

#endif
