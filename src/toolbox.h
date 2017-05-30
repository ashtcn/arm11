#ifndef TOOLBOX_H
#define TOOLBOX_H
#include <stdio.h>
#include "global.h"
#include "system_state.h"

void exit_program();
void print_array(void *p, size_t bytes_to_print);
word get_word(byte *memory, address mem_address);
word negate(word value);
bool is_negative(word value);
word absolute(word value);
void print_system_state(system_state machine);
void print_registers(system_state machine);
long twos_complement_to_long(word value);
void print_binary_value(word value);

#endif
