#ifndef TOOLBOX_H
#define TOOLBOX_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "global.h"
#include "system_state.h"
#include "value_carry.h"

void load_file(char *fname, byte_t *memory);
void exit_program(system_state_t *machine);

word_t get_word(system_state_t *machine, address_t mem_address);
word_t get_word_compliant(system_state_t *machine, address_t mem_address);
void set_word(system_state_t *machine, address_t mem_address, word_t word);

word_t negate(word_t value);
bool is_negative(word_t value);
word_t absolute(word_t value);

void print_array(byte_t *memory, size_t bytes_to_print);
void print_system_state(system_state_t *machine);
void print_system_state_compliant(system_state_t *machine);
void print_registers(system_state_t *machine);
void print_registers_compliant(system_state_t *machine);
void print_memory(system_state_t *machine);
void print_memory_compliant(system_state_t *machine);
void print_decoded_instruction(system_state_t *machine);
void print_fetched_instruction(system_state_t *machine);
void print_value(word_t value);
void print_value_compliant(word_t value);
char *get_cond(condition_t cond);
char *get_type(instruction_type_t type);
char *get_opcode(opcode_t operation);
long twos_complement_to_long(word_t value);
void print_binary_value(word_t value);

value_carry_t *shifter(shift_t type, word_t shift_amount, word_t value);

#endif
