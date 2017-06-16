/**
 * @file print.h
 * @brief Header file for print.c.
 */

#ifndef PRINT_H
#define PRINT_H
#include "../toolbox.h"

void print_array(byte_t *memory, size_t bytes_to_print);
void print_system_state(system_state_t *machine);
void print_instruction(instruction_t *instruction);

#endif
