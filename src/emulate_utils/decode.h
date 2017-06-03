/**
 * @file decode.h
 * @brief Header file for decode.c.
 */

#ifndef DECODE_H
#define DECODE_H
#include "../toolbox.h"

void decode_instruction(system_state_t *machine);
void halt(system_state_t *machine);
void branch(system_state_t *machine);
void single_data_transfer(system_state_t *machine);
void multiply(system_state_t *machine);
void data_processing(system_state_t *machine);

#endif
