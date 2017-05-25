#include <stdint.h>
#include "constants.h"

struct system_state {
  uint32_t registers[NUM_REGISTERS];
  unsigned char memory[NUM_ADDRESSES];
};
