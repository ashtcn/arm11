#include "assembler.h"

static const instruction_t NULL_INSTRUCTION = {
  .type = NUL,
  .immediate_value = 0,
  .rn = -1,
  .rd = -1,
  .rs = -1,
  .rm = -1,
  .flag_0 = false,
  .flag_1 = false,
  .flag_2 = false,
  .flag_3 = false,
  .shift_amount = 0,
};

mnemonic_t string_to_mnemonic(char *str) {
  // TODO: Maybe use hash? - Nikolai Smirnov
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
    return LSL_M;
  }
  if (!strcmp(str, "andeq")) {
    return ANDEQ_M;
  }

  fprintf(stderr, "No such mnemonic found.\n");
  exit(EXIT_FAILURE);
}

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

reg_address_t string_to_reg_address(char *str) {
  return strtol(&str[1], (char **) NULL, 10);
}

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

void parse_shift(string_array_t *tokens, instruction_t *instruction) {
  instruction->shift_type = string_to_shift(tokens->array[0]);
  if ('#' == tokens->array[1][0]) {
    // In the form <#expression>
    char *number = &tokens->array[1][1];
    instruction->shift_amount = strtol(number, (char **)NULL, 16);
  } else {
    // Is a register
    instruction->rs = string_to_reg_address(tokens->array[1]);
  }
}

void parse_operand(string_array_t *tokens, instruction_t *instruction) {
  char **sections = tokens->array;
  if ('#' == sections[0][0]) {
    // In the form <#expression>
    instruction->flag_0 = 1;
    if (strstr(sections[0], "0x")) {
      // Is in hex
      instruction->immediate_value = strtol(&sections[0][1], (char **)NULL, 16);
    } else {
      // Is in decimal
      instruction->immediate_value = strtol(&sections[0][1], (char **)NULL, 10);
    }

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

word_t assemble_dpi(string_array_t *tokens) {
  instruction_t *instruction = malloc(sizeof(instruction_t));
  *instruction = NULL_INSTRUCTION;
  char **sections = tokens->array;

  instruction->type = DPI;
  instruction->cond = AL;
  instruction->operation = mnemonic_to_opcode(string_to_mnemonic(sections[0]));

  string_array_t *operand_tokens = malloc(sizeof(string_array_t));
  if (!operand_tokens) {
    perror("Unable to allocate memory for operand_tokens.\n");
    exit(EXIT_FAILURE);
  }

  switch (instruction->operation) {
    case AND:
    case EOR:
    case SUB:
    case RSB:
    case ADD:
    case ORR:
      instruction->rd = string_to_reg_address(sections[1]);
      instruction->rn = string_to_reg_address(sections[2]);

      operand_tokens->array = &sections[3];
      operand_tokens->size = tokens->size - 3;
      parse_operand(operand_tokens, instruction);
      break;
    case MOV:
      instruction->rd = string_to_reg_address(sections[1]);

      operand_tokens->array = &sections[2];
      operand_tokens->size = tokens->size - 2;
      parse_operand(operand_tokens, instruction);
      break;
    case TST:
    case TEQ:
    case CMP:
      instruction->rn = string_to_reg_address(sections[1]);
      instruction->flag_1 = true;
      operand_tokens->array = &sections[2];
      operand_tokens->size = tokens->size - 2;
      parse_operand(operand_tokens, instruction);
      break;
  }

  free(operand_tokens);
  print_instruction(instruction);
  return encode(instruction);
}

word_t assemble_mul(string_array_t *tokens) {
  instruction_t *instruction = malloc(sizeof(instruction_t));
  *instruction = NULL_INSTRUCTION;

  instruction->type = MUL;
  instruction->cond = AL;
  instruction->rd = string_to_reg_address(tokens->array[1]);
  instruction->rm = string_to_reg_address(tokens->array[2]);
  instruction->rs = string_to_reg_address(tokens->array[3]);

  if (5 == tokens->size) {
    // Is an MLA instruction
    instruction->flag_0 = 1;
  }
  print_instruction(instruction);
  return encode(instruction);
}

void assemble_all_instructions(string_array_array_t *instructions, symbol_table_t *symbol_table, word_array_t *words) {
  word_array_t *extra_words = make_word_array();
  //int max_lines = instructions->size - symbol_table->size;
  word_t machine_instruction;
  for (int i = 0; i < instructions->size; i++) {
    if(instructions->string_arrays[i]->size != 1) {
      mnemonic_t ins_code = string_to_mnemonic(instructions->string_arrays[i]->array[0]);
      switch(ins_code) {
        case ADD_M:
        case SUB_M:
        case RSB_M:
        case AND_M:
        case EOR_M:
        case ORR_M:
        case MOV_M:
        case TST_M:
        case TEQ_M:
        case CMP_M:
          machine_instruction = assemble_dpi(instructions->string_arrays[i]);
          break;
        case MUL_M:
        case MLA_M:
          machine_instruction = assemble_mul(instructions->string_arrays[i]);
          break;
        case LDR_M:
        case STR_M:
          //SDT (use extra_words)
          machine_instruction = 0;
          break;
        case BEQ_M:
        case BNE_M:
        case BGE_M:
        case BLT_M:
        case BGT_M:
        case BLE_M:
        case B_M:
          //BRANCH
          machine_instruction = 0;
          break;
        case LSL_M:
        case ANDEQ_M:
          //SPECIAL
          machine_instruction = 0;
          break;
        default:
          fprintf(stderr, "No such opcode found.\n");
          exit(EXIT_FAILURE);
          break;
      }
      add_word_array(words, machine_instruction);
    }
  }
  //Add extra words from LDR
  for (int i = 0; i < extra_words->size; i++) {
    add_word_array(words, extra_words->array[i]);
  }
}
