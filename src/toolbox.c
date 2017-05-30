#include <stdlib.h>
#include "toolbox.h"

void exit_program(system_state_t *machine) {
  print_system_state(machine);
  exit(EXIT_FAILURE);
}

void print_array(void *p, size_t bytes_to_print) {
  byte_t *memory = (byte_t *) p;
  for (size_t i = 0; i < bytes_to_print; i++) {
    printf("%x", memory[i]);

    // New line at every 4 bytes
    if (i % 4 == 3) {
      printf("\n");
    }
  }
  printf("\n");
}

word_t get_word(system_state_t *machine, address_t mem_address) {
  word_t value = 0;
  for (size_t i = 0; i < 4; i++) {
    value |= ((word_t) machine->memory[mem_address + i]) << i*8;
  }
  return value;
}

word_t negate(word_t value) {
  return (~value) + 1;
}

bool is_negative(word_t value) {
  return value >> 31;
}

word_t absolute(word_t value) {
  if (is_negative(value)) {
    return negate(value);
  }
  return value;
}


void print_system_state(system_state_t *machine) {
  print_registers(machine);
  print_memory(machine);
}

void print_registers(system_state_t *machine) {
  printf("Register State:\n");
  for (int i = 0; i < NUM_REGISTERS; ++i) {
    printf("Register no: %d, has decimal value: %ld\n", i, twos_complement_to_long(machine->registers[i]));
    printf("Register no: %d, has binary value: ", i);
    print_binary_value(machine->registers[i]);
    printf("\n");
  }
}

void print_memory(system_state_t *machine) {
  printf("Memory state:\n");
  for (int i = 0; i < (NUM_ADDRESSES/4); i += 4) {
    word_t word_to_print = get_word(machine, i);
    if (word_to_print) {
      printf("Memory adress: %d, has hex value: %x\n", i, word_to_print);
      printf("Memory adress: %d, has decimal value: %ld\n", i, twos_complement_to_long(word_to_print));
      printf("Memory adress: %d, has binary value: ", i);
      print_binary_value(word_to_print);
      printf("\n");
    }
  }
}

long twos_complement_to_long(word_t value) {
  long result = absolute(value);
  if (is_negative(value)) {
    result *= -1;
  }
  return result;
}

void print_binary_value(word_t value) {
  for (int i = 0; i < WORD_SIZE; ++i) {
    printf("%u", value >> (WORD_SIZE - 1));
    value <<= 1;
  }
}

value_carry_t shifter(shift_t type, word_t shift_amount, word_t value) {
  switch(type) {
    case lsl:
      return value << shift_amount;
    case lsr:
      return value >> shift_amount;
    case asr:
      return (value >> shift_amount) | ((value >> 31) ? ~((1L << (3L - shift_amount)) - 1L) : 0L);
    case ror:
      return (value << (WORD_SIZE - shift_amount)) | (value >> shift_amount);
    default:
      fprintf(stderr, "Unknown shift type: %u", type);
      exit(EXIT_FAILURE);
  }
}
