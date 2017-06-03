/**
 * @file execute.h
 * @brief Header file for execute.c.
 */

#ifndef EXECUTE_H
#define EXECUTE_H
#include "../toolbox.h"

void execute(system_state_t *machine);
void execute_dpi(system_state_t *machine);
void execute_mul(system_state_t *machine);
void execute_branch(system_state_t *machine);
void execute_sdt(system_state_t *machine);

#endif
