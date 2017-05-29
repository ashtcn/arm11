#include <stdio.h>
#include "system_state.c"

void exit_program();

unsigned long negate(unsigned long value);
unsigned long absolute(unsigned long value);
bool is_negative(unsigned long value);
long convert_twos_complement(unsigned long value);
void print_decimal_value(unsigned long value);
void print_registers(system_state machine);

void print_array(void *p, unsigned int bytes_to_print) {
  unsigned char *memory = (unsigned char *) p;
  for (size_t i = 0; i < bytes_to_print; i++) {
    printf("%i", memory[i]);

    // New line at every 4 bytes
    if (i % 16 == 15) {
      printf("\n");
    }
  }
  printf("\n");
}

unsigned long get_word(unsigned char *memory, unsigned int address) {
  unsigned long word = 0;
  for (size_t i = 3; i >= 0; i--) {
    word = ((unsigned long) memory[address + i * 4]) | (word << 4);
  }
  return word;
}

unsigned long negate(unsigned long value) {
  return (~value) + 1;
}

bool is_negative(unsigned long value) {
  return value >> 31;
}

unsigned long absolute(unsigned long value) {
  if (is_negative(value)) {
    return negate(value);
  }
  return value;
}


void print_system_state(system_state machine) {
  print_registers(machine);
}

void print_registers(system_state machine) {
  printf("Register State:\n");
  for (int i = 0; i < NUM_REGISTERS; ++i) {
    printf("Register no: %d, has decimal value: %d\n", i, convert_twos_complement(machine.registers[i]));
    printf("Register no: %d, has binary value: ", i);
    print_decimal_value(machine.registers[i]);
    printf("\n");
  }
}

long convert_twos_complement(unsigned long value) {
  long result = absolute(value);
  if (is_negative(value)) {
    result *= -1;
  }
  return result;
}

void print_decimal_value(unsigned long value) {
  unsigned long flag = 0x7FFFFFFF;
  for (int i = 0; i < WORD_SIZE; ++i) {
    printf("%u", (flag & value) >> (WORD_SIZE - 1));
    value <<= 1;
  }
}
