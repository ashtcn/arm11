#ifndef VALUE_CARRY_H
#define VALUE_CARRY_H
#include <stdbool.h>
#include "global.h"

typedef struct {
  word_t value;
  bool carry;
} value_carry_t;

#endif
