/**
 * @file toolbox.c
 * @brief Miscellaneous functions that are widely used throughout the code.
 */

#include "toolbox.h"

/**
 * @brief Loads a binary file into the memory.
 *
 * Writes the contents of the provided binary object code file to the memory,
 * starting at the provided location. Returns an error message and exits if the
 * file cannot be opened or cannot be read.
 * @param fname The filename containing object code to be loaded.
 * @param memory A pointer to the first byte of memory to be written to.
 */
void load_file(char *fname, byte_t *memory) {
   // Try to open the file
   FILE *file = fopen(fname, "rb");
   if (file == NULL) {
     perror("Error in opening object code file.");
     exit(EXIT_FAILURE);
   }
   // Try to read all lines into the memory
   size_t size = fread(memory, NUM_ADDRESSES, 1, file);
   if (ferror(file)) {
     printf("File size: %lu", size);
     perror("Error in reading from object code file.");
     exit(EXIT_FAILURE);
   }
   // Close the file
   fclose(file);
}

/**
 * @brief Exits gracefully.
 *
 * Prints the current system state, frees allocated memory and exits with a
 * failure. To be used in the case of an error which cannot be recovered from.
 * @param machine The current system state.
 */
void exit_program(system_state_t *machine) {
  print_system_state(machine);
  free(machine->decoded_instruction);
  free(machine);
  exit(EXIT_FAILURE);
}

/**
 * @brief Gets a memory word from a given address.
 *
 * * If GPIO access adddress is read, prints a message to stdout.
 * * If another out of bounds address is read, prints an error.
 * @param machine The current system state.
 * @param mem_address The memory address to be read from.
 * @returns The word at the given memory address in the current system state.
 */
word_t get_word(system_state_t *machine, uint32_t mem_address) {
  if (mem_address >= GPIO_ACCESS_START
    && mem_address < GPIO_ACCESS_START + GPIO_ACCESS_SIZE) {
    // GPIO pin accessed
    printf("One GPIO pin from %u to %u has been accessed\n",
           (mem_address - GPIO_ACCESS_START) / 4 * 10,
           (mem_address - GPIO_ACCESS_START) / 4 * 10 + 9);
    return mem_address;
  } else if (mem_address > NUM_ADDRESSES - 4) {
    // Out of bounds memory access
    if (COMPLIANT_MODE) {
      printf("Error: Out of bounds memory access at address 0x%08x\n",
             mem_address);
      return 0;
    } else {
      fprintf(stderr, "Address: 0x%x was out of bounds in get_word",
              mem_address);
      exit_program(machine);
    }
  }
  word_t value = 0;
  for (size_t i = 0; i < 4; i++) {
    value |= ((word_t) machine->memory[mem_address + i]) << (i * 8);
  }
  return value;
}

/**
 * @brief Gets a memory word from a given address (for printing only).
 *
 * For use in compliant printing. Gets the word in little endian order.
 * @param machine The current system state.
 * @param mem_address The memory address to be read from.
 * @returns The word at the given memory address in the current system state.
 */
word_t get_word_compliant(system_state_t *machine, address_t mem_address) {
  word_t value = 0;
  for (size_t i = 0; i < 4; i++) {
    value |= ((word_t) machine->memory[mem_address + 3 - i]) << (i * 8);
  }
  return value;
}

/**
 * @brief Writes a word to memory at a given address.
 *
 * * If GPIO access adddress is written to, prints a message to stdout.
 * * If GPIO clear or set adddress is written to, prints a message to stdout.
 * * If another out of bounds address is read, prints an error.
 * @param machine The current system state.
 * @param mem_address The memory address to write to.
 * @param word The word to write to memory.
 */
void set_word(system_state_t *machine, uint32_t mem_address, word_t word) {
  if (mem_address >= GPIO_ACCESS_START
    && mem_address < GPIO_ACCESS_START + GPIO_ACCESS_SIZE) {
    // GPIO pin accessed
    printf("One GPIO pin from %u to %u has been accessed\n",
           (mem_address - GPIO_ACCESS_START) / 4 * 10,
           (mem_address - GPIO_ACCESS_START) / 4 * 10 + 9);
    return;
  } else if (mem_address >= GPIO_CLEAR_START
    && mem_address < GPIO_CLEAR_START + GPIO_CLEAR_SIZE) {
    // GPIO pin cleared
    printf("PIN OFF\n");
    return;
  } else if (mem_address >= GPIO_SET_START
    && mem_address < GPIO_SET_START + GPIO_SET_SIZE) {
    // GPIO pin set
    printf("PIN ON\n");
    return;
  } else if (mem_address > NUM_ADDRESSES - 4) {
    // Out of bounds memory access
    if (COMPLIANT_MODE) {
      printf("Error: Out of bounds memory access at address 0x%x\n",
             mem_address);
      return;
    } else {
      fprintf(stderr, "Address: 0x%x was out of bounds in set_word",
              mem_address);
      exit_program(machine);
    }
  }
  for (size_t i = 0; i < 4; i++) {
    machine->memory[mem_address + i] = (byte_t) (word & 0xFF);
    word >>= 8;
  }
}

/**
 * @brief Negates a two's complement value.
 *
 * @param value The word to be negated.
 * @returns The negated word.
 */
word_t negate(word_t value) {
  return (~value) + 1;
}

/**
 * @brief Returns true iff two's complement value is negative.
 *
 * @param value A two's complement word to check sign of.
 * @returns True iff provided value is negative in two's complement.
 */
bool is_negative(word_t value) {
  return value >> 31;
}

/**
 * @brief Returns absolute two's complement value.
 *
 * @param value A two's complement word.
 * @returns The absolute value of the provided word in two's complement.
 */
word_t absolute(word_t value) {
  if (is_negative(value)) {
    return negate(value);
  }
  return value;
}

/**
 * @brief Returns a two's complement representation for the given value.
 *
 * @param value The value to convert.
 * @returns A 32-bit two's complement representation.
 */
uint32_t signed_to_twos_complement(int32_t value) {
  uint32_t result = abs(value);
  if (value < 0) {
    result = negate(result);
  }
  return result;
}

/**
 * @brief Converts a signed 2's complement word to a sign long.
 *
 * @param value The signed 2's complement word to convert.
 * @returns The signed long representation of the word.
 */
long twos_complement_to_long(word_t value) {
  long result = absolute(value);

  if (is_negative(value)) {
    result *= -1;
  }
  return result;
}

/**
 * @brief Shifts a value and returns a pointer.
 *
 * @param type The type of shift to use.
 * @param shift_amount The amount to shift by.
 * @param value The value to shift.
 * @returns The pointer to the shifted value.
 */
 value_carry_t *shifter(shift_t type, word_t shift_amount, word_t value) {
   value_carry_t *result = malloc(sizeof(value_carry_t));

   if (!result) {
     perror("Unable to allocate memory for result of shifter");
     exit(EXIT_FAILURE);
   }

   switch(type) {
     case LSL:
       result->value = (shift_amount >= WORD_SIZE) ? 0 : value << shift_amount;
       result->carry = (value >> (WORD_SIZE - shift_amount)) & 0x1;
       break;
     case LSR:
       result->value = (shift_amount >= WORD_SIZE) ? 0 : value >> shift_amount;
       result->carry = (value << (WORD_SIZE - shift_amount)) & 0x80000000;
       break;
     case ASR:
       result->value = (value >> shift_amount)
                       | ((value & 0x80000000) ?
                         ~((1L << (WORD_SIZE - shift_amount)) - 1L) : 0L);
       result->carry = (value << (WORD_SIZE - shift_amount)) & 0x80000000;
       break;
     case ROR:
       result->value = (value << (WORD_SIZE - shift_amount))
                       | (value >> shift_amount);
       result->carry = (value << (WORD_SIZE - shift_amount)) & 0x80000000;
       break;
     default:
       fprintf(stderr, "Unknown shift type: %u", type);
       exit(EXIT_FAILURE);
   }

   if (shift_amount == 0) {
     result->carry = false;
   }

   return result;
 }
