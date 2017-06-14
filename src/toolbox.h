/**
 * @file toolbox.h
 * @brief Header file for toolbox.c.
 */

#ifndef TOOLBOX_H
#define TOOLBOX_H
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "emulate_utils/system_state.h"
#include "emulate_utils/value_carry.h"
#include "emulate_utils/print.h"

void load_file(char *fname, byte_t *memory);
void exit_program(system_state_t *machine);

word_t get_word(system_state_t *machine, uint32_t mem_address);
word_t get_word_compliant(system_state_t *machine, address_t mem_address);
void set_word(system_state_t *machine, uint32_t mem_address, word_t word);

word_t negate(word_t value);
bool is_negative(word_t value);
word_t absolute(word_t value);
uint32_t signed_to_twos_complement(int32_t value);
long twos_complement_to_long(word_t value);

value_carry_t *shifter(shift_t type, word_t shift_amount, word_t value);

#endif
