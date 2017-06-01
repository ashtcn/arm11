/** A header to define the value_carry_t type. */

#ifndef VALUE_CARRY_H
#define VALUE_CARRY_H
#include <stdbool.h>
#include "global.h"

/**
 * @brief A struct that has a value and a carry.
 */
typedef struct {
  /** The value. */
  word_t value;
  /** Whether or not there is a carry bit present. */
  bool carry;
} value_carry_t;

#endif
