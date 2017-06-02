/**
 * @file print.c
 * @brief Functions for printing system details to standard output.
 */

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
 * * For multiply instructions, prints the condition, flags and registers.
 * * For data processing instructions, prints the condition, flags, opcodes,
 * operands, and shift information.
 * * For single data transfer instructions, prints flags, registers and offset.
 * @param machine The current system state.
 */
 void print_decoded_instruction(system_state_t *machine) {
   print_instruction(machine->decoded_instruction);
 }

 /**
  * @brief Prints details for the instruction.
  *
  * Prints the type of the instruction, and any details required:
  * * For branch instructions, prints the condition and the offset.
  * * For multiply instructions, prints the condition, flags and registers.
  * * For data processing instructions, prints the condition, flags, opcodes,
  * operands, and shift information.
  * * For single data transfer instructions, prints flags, registers and offset.
  * @param instruction The instruction.
  */
void print_instruction(instruction_t *instruction) {
  switch (instruction->type) {
    case NUL:
      printf("Decoded Instruction: None\n");
      break;
    case ZER:
      printf("Decoded Instruction: ZER\n");
      break;
    case BRA:
      printf("Decoded Instruction: BRA\n");
      printf("  Condition Flag: %s\n", get_cond(instruction->cond));
      printf("  Offset: 0x%x\n", instruction->immediate_value);
      break;
    case MUL:
      printf("Decoded Instruction: MUL\n");
      printf("  Condition Flag: %s\n", get_cond(instruction->cond));
      printf("  Accumulate: %u\n", instruction->flag_0);
      printf("  Set Flags: %u\n", instruction->flag_1);
      printf("  Operand1 Register Rm: %u\n", instruction->rm);
      printf("  Operand2 Register Rs: %u\n", instruction->rs);
      if(instruction->flag_0) {
        // Accumulate is set
        printf("  Accumulate Register Rn: %u\n", instruction->rn);
      }
      printf("  Destination Register Rd: %u\n", instruction->rd);
      break;
    case DPI:
      printf("Decoded Instruction: DPI\n");
      printf("  Condition Flag: %s\n", get_cond(instruction->cond));
      printf("  Opcode: %s\n", get_opcode(instruction->operation));
      printf("  Immediate Operand: %u\n", instruction->flag_0);
      printf("  Set Flags: %u\n", instruction->flag_1);
      printf("  Operand1 Register Rn: %u\n", instruction->rn);
      if(instruction->flag_0) {
        // Operand2 is immediate
        printf("  Operand2 Immediate Value: 0x%x\n",
               instruction->immediate_value);
        printf("  Operand2 Rotate Right Amount: %u\n",
               instruction->shift_amount);
      } else {
        // Operand2 is register
        printf("  Operand2 Register Rm: %u\n", instruction->rm);
        printf("  Operand2 Shift Type: %s\n",
               get_shift(instruction->shift_type));
        if (instruction->rs == -1) {
          // Shift is immediate
          printf("  Operand2 Shift Amount: %u\n", instruction->shift_amount);
        } else {
          // Shift is register
          printf("  Operand2 Shift Register Rs: %u\n", instruction->rs);
        }
      }
      printf("  Destination Register Rd: %u\n", instruction->rd);
      break;
    case SDT:
      printf("Decoded Instruction: SDT\n");
      printf("  Condition Flag: %s\n", get_cond(instruction->cond));
      printf("  Immediate Offset: %u\n", instruction->flag_0);
      printf("  Pre (1) or Post Indexing (0): %u\n", instruction->flag_1);
      printf("  Offset Add (1) or Subtract (0): %u\n", instruction->flag_2);
      printf("  Load (1) or Store (0): %u\n", instruction->flag_3);
      printf("  Base Register Rn: %u\n", instruction->rn);
      if(instruction->flag_0) {
        // Offset is register
        printf("  Offset Register Rm: %u\n", instruction->rm);
        printf("  Offset Shift Type: %s\n",
               get_shift(instruction->shift_type));
        if (instruction->rs == -1) {
          // Shift is immediate
          printf("  Offset Shift Amount: %u\n", instruction->shift_amount);
        } else {
          // Shift is register
          printf("  Offset Shift Register Rs: %u\n", instruction->rs);
        }
      } else {
        // Offset is immediate
        printf("  Offset Immediate Value: 0x%x\n",
               instruction->immediate_value);
      }
      printf("  Source / Destination Register Rd: %u\n", instruction->rd);
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
 * @param cond The condition type.
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

/**
 * @brief Returns the string representing the shift type.
 *
 * @param operation The type of shift.
 * @returns The string of the type of shift for printing.
 */
char *get_shift(shift_t shift) {
  switch (shift) {
    case LSL:
      return "LSL";
    case LSR:
      return "LSR";
    case ASR:
      return "ASR";
    case ROR:
      return "ROR";
    default:
      assert(false);
  }
}
