#ifndef PRINT_COMPLIANT_H
#define PRINT_COMPLIANT_H
#include "print.h"

void print_system_state_compliant(system_state_t *machine);
void print_registers_compliant(system_state_t *machine);
void print_memory_compliant(system_state_t *machine);
void print_value_compliant(word_t value);

#endif
