#include "toolbox.h"

/**
 * @brief Loads a binary file into the memory.
 *
 * Writes the contents of the provided binary object code file to the memory,
 * starting at the provided location. Returns an error message and exits if the
 * file cannot be opened or cannot be read.
 * @param fname The filename conataining object code to be loaded.
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
   fread(memory, NUM_ADDRESSES, 1, file);
   if(ferror(file)) {
     perror("Error in reading from object code file.");
     exit(EXIT_FAILURE);
   }
   // Close the file
   fclose(file);
}

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
    value |= ((word_t) machine->memory[mem_address + i]) << (i * 8);
  }
  return value;
}

void set_word(system_state_t *machine, address_t mem_address, word_t word) {
  for (size_t i = 0; i < 4; i++) {
    machine->memory[mem_address + i] = (byte_t) (word & 0xF);
    word >>= 4;
  }
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
  for (uint8_t i = 0; i < NUM_REGISTERS; ++i) {
    word_t value = machine->registers[i];
    printf("Register no: %d, ", i);
    print_value(value);
  }
}

void print_memory(system_state_t *machine) {
  printf("Memory state:\n");
  for (uint32_t i = 0; i < NUM_ADDRESSES; i += 4) {
    word_t value = get_word(machine, i);
    if (value) {
      printf("Memory address: %d, ", i);
      print_value(value);
    }
  }
}

void print_value(word_t value) {
  printf("Value: ");
  print_binary_value(value);
  printf(" (0x%x) (%ld)\n", value, twos_complement_to_long(value));
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

value_carry_t *shifter(shift_t type, word_t shift_amount, word_t value) {
  value_carry_t *result = malloc(sizeof(value_carry_t));
  if (!result) {
    fprintf(stderr, "Unable to allocate memory for result of shifter");
    exit(EXIT_FAILURE);
  }
  switch(type) {
    case lsl:
      result->value = value << shift_amount;
      result->carry = (value >> (WORD_SIZE - shift_amount)) & 0x1;
      break;
    case lsr:
      result->value = value >> shift_amount;
      result->carry = value << (WORD_SIZE - shift_amount) & 0x1;
      break;
    case asr:
      result->value = (value >> shift_amount) | ((value >> 31) ? ~((1L << (3L - shift_amount)) - 1L) : 0L);
      result->carry = value << (WORD_SIZE - shift_amount) & 0x1;
      break;
    case ror:
      result->value = (value << (WORD_SIZE - shift_amount)) | (value >> shift_amount);
      result->carry = value << (WORD_SIZE - shift_amount) & 0x1;
      break;
    default:
      fprintf(stderr, "Unknown shift type: %u", type);
      exit(EXIT_FAILURE);
  }
  return result;
}

void decode_instruction(system_state_t *machine) {
  // PRE: Instruction is not all 0
  word_t fetched = machine->fetched_instruction;
  instruction_t *instruction = machine->decoded_instruction;
  instruction->cond = fetched >> (WORD_SIZE - 4);

  // No longer consider Cond, so remove it
  fetched &= MASK_FIRST_4;

  if ((fetched >> (WORD_SIZE - 8)) == 0xA) { // Branch iff 1010 (0xA)
    // Branch
    branch(machine);
  } else if ((fetched >> (WORD_SIZE - 6)) == 0x1) {
    // Single Data Transfer
    multiply(machine);
  } else if (!(fetched >> 22) && (((fetched >> 4) & 0xF) == 0x9)) {
    //Multiply
    single_data_transfer(machine);
  } else if (!(fetched >> (WORD_SIZE - 6))) {
    // Data Processing
    data_processing(machine);
  } else {
    // Unknown instruction
    fprintf(stderr, "Unknown instruction, PC: %u", machine->registers[PC]);
    exit_program(machine);
  }
}

void branch(system_state_t *machine) {
  machine->decoded_instruction->immediate_value = machine->fetched_instruction
                                                & 0xFFFFFF; // First 24 bits
}

void multiply(system_state_t *machine) {
  instruction_t *instruction = machine->decoded_instruction;
  word_t fetched = machine->fetched_instruction;

  instruction->type = MUL;
  instruction->flag_0 = (fetched >> 21) & 0x1;
  instruction->flag_1 = (fetched >> 20) & 0x1;
  instruction->rn = (fetched >> 12) & 0x4;
  instruction->rd = (fetched >> 16) & 0x4;
  instruction->rs = (fetched >> 8) & 0x4;
  instruction->rm = fetched & 0x4;
}

void single_data_transfer(system_state_t *machine) {

}

void data_processing(system_state_t *machine) {
  instruction_t *instruction = machine->decoded_instruction;
  word_t fetched = machine->fetched_instruction;

  instruction->type = DPI;
  instruction->flag_0 = (fetched >> 25) & 0x1;
  instruction->flag_1 = (fetched >> 20) & 0x1;
  instruction->operation = (fetched >> 21) & 0x4;
  instruction->rn = (fetched >> 16) & 0x4;
  instruction->rd = (fetched >> 12) & 0x4;
  instruction->immediate_value = fetched & 0xC;
}
