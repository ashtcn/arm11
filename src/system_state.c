#ifndef SYSTEM_STATE_H_INCLUDED
#define SYSTEM_STATE_H_INCLUDED
#include <stdint.h>
#include "constants.h"

struct system_state {
  uint32_t registers[NUM_REGISTERS] = {0};
  unsigned char memory[NUM_ADDRESSES] = {0};
};

#endif
