#include "print.h"

/**
 * @brief Prints a given number of bytes, from an array of bytes.
 *
 * Prints a given number bytes from memory, starting from address 0. Lines are
 * broken every word (4 bytes). Useful for debugging.
 * @param memory An array of bytes to print.
 * @param bytes_to_print The number of bytes to print (from 0).
 */
void print_array(byte_t *memory, size_t bytes_to_print) {
  for (size_t i = 0; i < bytes_to_print; i++) {
    printf("%x", memory[i]);
    // New line at each word
    if (i % 4 == 3) {
      printf("\n");
    }
  }
  printf("\n");
}

/**
 * @brief Prints system state details.
 *
 * Prints the current system state. Prints all register values, any memory
 * values which are not 0, the decoded instruction and the fetched instruction.
 * @param machine The current system state.
 */
void print_system_state(system_state_t *machine) {
  printf("\n--------------------------------------------------\n\n");
  printf("System State:\n");
  print_registers(machine);
  print_memory(machine);
  print_decoded_instruction(machine);
  print_fetched_instruction(machine);
}

/**
 * @brief Prints the values of registers.
 *
 * Prints the values held in each of the NUM_REGISTERS registers.
 * @param machine The current system state.
 */
void print_registers(system_state_t *machine) {
  printf("Register State:\n");
  for (uint8_t i = 0; i < NUM_REGISTERS; ++i) {
    word_t value = machine->registers[i];
    printf("Register %2d, ", i);
    print_value(value);
  }
}

/**
 * @brief Prints any non-zero words from memory.
 *
 * Prints any non-zero words from memory and their addresses.
 * @param machine The current system state.
 */
void print_memory(system_state_t *machine) {
  printf("Memory State:\n");
  for (uint32_t i = 0; i < NUM_ADDRESSES; i += 4) {
    word_t value = get_word(machine, i);
    if (value) {
      printf("Memory Address %5d, ", i);
      print_value(value);
    }
  }
}


/**
 * @brief Prints details for the decoded instruction.
 *
 * Prints the type of the instruction, and any details required:
 * * For branch instructions, prints the condition and the offset.
 * *
 * @param machine The current system state.
 */
void print_decoded_instruction(system_state_t *machine) {
  switch (machine->decoded_instruction->type) {
    case NUL:
      printf("Decoded Instruction: None\n");
      break;
    case ZER:
      printf("Decoded Instruction: ZER\n");
      break;
    case BRA:
      printf("Decoded Instruction: BRA\n");
      printf("  Condition Flag: %s\n",
             get_cond(machine->decoded_instruction->cond));
      printf("  Offset: 0x%x\n",
             machine->decoded_instruction->immediate_value);
      break;
    case MUL:
      printf("Decoded Instruction: MUL\n");
      printf("  Condition Flag: %s\n",
             get_cond(machine->decoded_instruction->cond));
      if (machine->decoded_instruction->type == DPI) {
        printf("  Opcode: %s\n",
               get_opcode(machine->decoded_instruction->operation));
      }
      printf("  Immediate Value: 0x%x\n",
             machine->decoded_instruction->immediate_value);
      printf("  Destination Register: %d\n", machine->decoded_instruction->rd);
      break;
    case DPI:
      printf("Decoded Instruction: DPI\n");
      printf("  Condition Flag: %s\n",
             get_cond(machine->decoded_instruction->cond));
      if (machine->decoded_instruction->type == DPI) {
        printf("  Opcode: %s\n",
               get_opcode(machine->decoded_instruction->operation));
      }
      printf("  Immediate Value: 0x%x\n",
             machine->decoded_instruction->immediate_value);
      printf("  Destination Register: %d\n", machine->decoded_instruction->rd);
      break;
    case SDT:
      printf("Decoded Instruction: SDT\n");
      printf("  Condition Flag: %s\n",
             get_cond(machine->decoded_instruction->cond));
      if (machine->decoded_instruction->type == DPI) {
        printf("  Opcode: %s\n",
               get_opcode(machine->decoded_instruction->operation));
      }
      printf("  Immediate Value: 0x%x\n",
             machine->decoded_instruction->immediate_value);
      printf("  Destination Register: %d\n", machine->decoded_instruction->rd);
      break;
    default:
      assert(false);
  }
}

/**
 * @brief Prints the fetched instruction, if present.
 *
 * @param machine The current system state.
 */
void print_fetched_instruction(system_state_t *machine) {
  if (machine->has_fetched_instruction) {
    printf("Fetched Instruction, ");
    print_value(machine->fetched_instruction);
  } else {
    printf("Fetched Instruction: None\n");
  }
}


/**
 * @brief Prints a value for debugging, in binary, hex and 2's complement.
 *
 * @param value The word to print.
 */
void print_value(word_t value) {
  printf("Value: ");
  print_binary_value(value);
  printf(" (0x%08x) (%ld)\n", value, twos_complement_to_long(value));
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
 * @brief Prints the padded binary representation of value.
 *
 * Prints WORD_SIZE bits.
 * @param value The word for printing.
 */
void print_binary_value(word_t value) {
  for (int i = 0; i < WORD_SIZE; ++i) {
    printf("%u", value >> (WORD_SIZE - 1));
    value <<= 1;
  }
}

/**
 * @brief Returns the string representing the condition type.
 *
 * @param type The condition type.
 * @returns The string of the condition type for printing.
 */
char *get_cond(condition_t cond) {
  switch (cond) {
    case EQ:
      return "EQ";
    case NE:
      return "NE";
    case GE:
      return "GE";
    case LT:
      return "LT";
    case GT:
      return "GT";
    case LE:
      return "LE";
    case AL:
      return "AL";
    default:
      assert(false);
  }
}

/**
 * @brief Returns the string representing the instruction type.
 *
 * @param type The instruction type.
 * @returns The string of the instruction type for printing.
 */
char *get_type(instruction_type_t type) {
  switch (type) {
    case DPI:
      return "DPI";
    case MUL:
      return "MUL";
    case SDT:
      return "SDT";
    case BRA:
      return "BRA";
    case ZER:
      return "ZER";
    case NUL:
      return "NUL";
    default:
      assert(false);
  }
}

/**
 * @brief Returns the string representing the opcode.
 *
 * @param operation The opcode.
 * @returns The string of the opcode for printing.
 */
char *get_opcode(opcode_t operation) {
  switch (operation) {
    case AND:
      return "AND";
    case EOR:
      return "EOR";
    case SUB:
      return "SUB";
    case RSB:
      return "RSB";
    case ADD:
      return "ADD";
    case TST:
      return "TST";
    case TEQ:
      return "TEQ";
    case CMP:
      return "CMP";
    case ORR:
      return "ORR";
    case MOV:
      return "MOV";
    default:
      assert(false);
  }
}
