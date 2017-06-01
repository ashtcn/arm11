#ifndef TOOLBOX_H
#define TOOLBOX_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "global.h"
#include "system_state.h"
#include "value_carry.h"
#include "print.h"

void load_file(char *fname, byte_t *memory);
void exit_program(system_state_t *machine);

word_t get_word(system_state_t *machine, uint32_t mem_address);
word_t get_word_compliant(system_state_t *machine, address_t mem_address);
void set_word(system_state_t *machine, uint32_t mem_address, word_t word);

word_t negate(word_t value);
bool is_negative(word_t value);
word_t absolute(word_t value);

value_carry_t *shifter(shift_t type, word_t shift_amount, word_t value);

#endif
