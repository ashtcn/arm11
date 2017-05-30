#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H
#include <stdint.h>
#include "global.h"

typedef struct {
  word registers[NUM_REGISTERS] = {0};
  byte memory[NUM_ADDRESSES] = {0};
} system_state;

#endif
