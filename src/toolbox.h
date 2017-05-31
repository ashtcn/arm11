#ifndef TOOLBOX_H
#define TOOLBOX_H
#include <stdlib.h>
#include <stdio.h>
#include "global.h"
#include "system_state.h"
#include "value_carry.h"

void load_file(char *fname, byte_t *memory);
void exit_program(system_state_t *machine);
void print_array(void *p, size_t bytes_to_print);
word_t get_word(system_state_t *machine, address_t mem_address);
void set_word(system_state_t *machine, address_t mem_address, word_t word);
word_t negate(word_t value);
bool is_negative(word_t value);
word_t absolute(word_t value);
void print_system_state(system_state_t *machine);
void print_registers(system_state_t *machine);
void print_memory(system_state_t *machine);
void print_value(word_t value);
long twos_complement_to_long(word_t value);
void print_binary_value(word_t value);
value_carry_t *shifter(shift_t type, word_t shift_amount, word_t value);

#endif
