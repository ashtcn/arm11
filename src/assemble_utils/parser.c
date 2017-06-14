/**
 * @file parser.c
 * @brief File for functions used to parse an assembly input.
 */

#include "parser.h"

/**
 * @brief Returns the mnemonic for a given operator string.
 *
 * @param str An operator string.
 * @returns A mnemonic_t representing the given string.
 */
mnemonic_t string_to_mnemonic(char *str) {
  if (!strcmp(str, "add")) {
    return ADD_M;
  }
  if (!strcmp(str, "sub")) {
    return SUB_M;
  }
  if (!strcmp(str, "rsb")) {
    return RSB_M;
  }
  if (!strcmp(str, "and")) {
    return AND_M;
  }
  if (!strcmp(str, "eor")) {
    return EOR_M;
  }
  if (!strcmp(str, "orr")) {
    return ORR_M;
  }
  if (!strcmp(str, "mov")) {
    return MOV_M;
  }
  if (!strcmp(str, "tst")) {
    return TST_M;
  }
  if (!strcmp(str, "teq")) {
    return TEQ_M;
  }
  if (!strcmp(str, "cmp")) {
    return CMP_M;
  }
  if (!strcmp(str, "mul")) {
    return MUL_M;
  }
  if (!strcmp(str, "mla")) {
    return MLA_M;
  }
  if (!strcmp(str, "ldr")) {
    return LDR_M;
  }
  if (!strcmp(str, "str")) {
    return STR_M;
  }
  if (!strcmp(str, "beq")) {
    return BEQ_M;
  }
  if (!strcmp(str, "bne")) {
    return BNE_M;
  }
  if (!strcmp(str, "bge")) {
    return BGE_M;
  }
  if (!strcmp(str, "blt")) {
    return BLT_M;
  }
  if (!strcmp(str, "bgt")) {
    return BGT_M;
  }
  if (!strcmp(str, "ble")) {
    return BLE_M;
  }
  if (!strcmp(str, "b")) {
    return B_M;
  }
  if (!strcmp(str, "lsl")) {
    return SHIFT_M;
  }
  if (!strcmp(str, "lsr")) {
    return SHIFT_M;
  }
  if (!strcmp(str, "asr")) {
    return SHIFT_M;
  }
  if (!strcmp(str, "ror")) {
    return SHIFT_M;
  }
  if (!strcmp(str, "andeq")) {
    return ANDEQ_M;
  }

  fprintf(stderr, "No such mnemonic found.\n");
  exit(EXIT_FAILURE);
}

/**
 * @brief Returns the condition for a given condition string.
 *
 * @param str A condition string.
 * @returns A condition_t representing the given string.
 */
condition_t string_to_condition(char *str) {
  if (!strcmp(str, "eq")) {
    return EQ;
  }
  if (!strcmp(str, "ne")) {
    return NE;
  }
  if (!strcmp(str, "ge")) {
    return GE;
  }
  if (!strcmp(str, "lt")) {
    return LT;
  }
  if (!strcmp(str, "gt")) {
    return GT;
  }
  if (!strcmp(str, "le")) {
    return LE;
  }
  return AL;
}

/**
 * @brief Returns the opcode for a given mnemonic.
 *
 * @param str A mnemonic.
 * @returns The opcode of the given mnemonic.
 */
opcode_t mnemonic_to_opcode(mnemonic_t mnemonic) {
  switch (mnemonic) {
    case ADD_M:
      return ADD;
      break;
    case SUB_M:
      return SUB;
      break;
    case RSB_M:
      return RSB;
      break;
    case AND_M:
      return AND;
      break;
    case EOR_M:
      return EOR;
      break;
    case ORR_M:
      return ORR;
      break;
    case MOV_M:
      return MOV;
      break;
    case TST_M:
      return TST;
      break;
    case TEQ_M:
      return TEQ;
      break;
    case CMP_M:
      return CMP;
      break;
    default:
      fprintf(stderr, "No such opcode found.\n");
      exit(EXIT_FAILURE);
      break;
  }
}

/**
 * @brief Returns the address for a given address string.
 *
 * @param str An address string.
 * @returns The register number given by the string.
 */
reg_address_t string_to_reg_address(char *str) {
  return strtol(&str[1], (char **) NULL, 10);
}

/**
 * @brief Returns the shift for a given shift string.
 *
 * @param str A shift string.
 * @returns A shift_t representing the given string.
 */
shift_t string_to_shift(char *str) {
  if (!strcmp(str, "lsl")) {
    return LSL;
  }
  if (!strcmp(str, "lsr")) {
    return LSR;
  }
  if (!strcmp(str, "asr")) {
    return ASR;
  }
  if (!strcmp(str, "ror")) {
    return ROR;
  }

  fprintf(stderr, "No such shift found.\n");
  exit(EXIT_FAILURE);
}

/**
 * @brief Parses the shift and adds it to the given instruction.
 *
 * @param tokens The string to parse.
 * @param instruction The instruction to add the shift to.
 */
void parse_shift(string_array_t *tokens, instruction_t *instruction) {
  instruction->shift_type = string_to_shift(tokens->array[0]);
  if ('#' == tokens->array[1][0]) {
    // In the form <#expression>
    char *number = &tokens->array[1][1];
    instruction->shift_amount = parse_immediate_value(number);
  } else {
    // Is a register
    instruction->rs = string_to_reg_address(tokens->array[1]);
  }
}

/**
 * @brief Parses the operand and adds it to the given instruction.
 *
 * There are many different cases for op2 in data processing instructions so
 * this function is called to parse them.
 *
 * @param tokens The string to parse.
 * @param instruction The instruction to add the operand to.
 */
void parse_operand(string_array_t *tokens, instruction_t *instruction) {
  char **sections = tokens->array;
  if ('#' == sections[0][0]) {
    // In the form <#expression>
    instruction->flag_0 = 1;
    instruction->immediate_value = parse_immediate_value(&sections[0][1]);

    uint16_t shift = WORD_SIZE;
    if (instruction->immediate_value > 0xFF) {
      while (!(instruction->immediate_value & 0x3)) {
          instruction->immediate_value >>= 2;
          shift--;
      }
    }

    instruction->shift_amount = shift;

  } else if ('r' == sections[0][0]) {
    // In the form Rm{,<shift>}
    instruction->rm = string_to_reg_address(sections[0]);

    if (tokens->size >= 2) {
      // Has shift
      string_array_t *shift_tokens = malloc(sizeof(string_array_t));

      if (!shift_tokens) {
        perror("Unable to allocate memory for shift_tokens.\n");
        exit(EXIT_FAILURE);
      }

      // Pass the <shift> section into parse_shift
      shift_tokens->array = &sections[1];
      shift_tokens->size = tokens->size - 1;
      parse_shift(shift_tokens, instruction);
      free(shift_tokens);
    }
  }
}

/**
 * @brief Parses an immediate value.
 *
 * This immediate value could be in hex or in decimal.
 *
 * @param str The string to parse.
 * @returns The immediate value.
 */
word_t parse_immediate_value(char *str) {
  if (strstr(str, "0x")) {
    // Is in hex
    return strtol(str, (char **)NULL, 16);
  } else {
    // Is in decimal
    return strtol(str, (char **)NULL, 10);
  }
}
