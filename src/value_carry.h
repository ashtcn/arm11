#ifndef VALUE_CARRY_H
#define VALUE_CARRY_H
#include <stdbool.h>
#include "global.h"

struct value_carry {
  word value;
  bool carry;
};

typedef struct value_carry value_carry_t;

#endif
