/** Header file for print.c. */

#ifndef PRINT_H
#define PRINT_H
#include "toolbox.h"

void print_array(byte_t *memory, size_t bytes_to_print);
void print_system_state(system_state_t *machine);
void print_registers(system_state_t *machine);
void print_memory(system_state_t *machine);
void print_decoded_instruction(system_state_t *machine);
void print_fetched_instruction(system_state_t *machine);
void print_instruction(instruction_t *instruction);
void print_value(word_t value);
char *get_cond(condition_t cond);
char *get_opcode(opcode_t operation);
char *get_shift(shift_t shift);
long twos_complement_to_long(word_t value);
void print_binary_value(word_t value);

#endif
