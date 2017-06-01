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
  if (strcmp(str, "add")) {
    return ADD_M;
  }
  if (strcmp(str, "sub")) {
    return SUB_M;
  }
  if (strcmp(str, "rsb")) {
    return RSB_M;
  }
  if (strcmp(str, "and")) {
    return AND_M;
  }
  if (strcmp(str, "eor")) {
    return EOR_M;
  }
  if (strcmp(str, "orr")) {
    return ORR_M;
  }
  if (strcmp(str, "mov")) {
    return MOV_M;
  }
  if (strcmp(str, "tst")) {
    return TST_M;
  }
  if (strcmp(str, "teq")) {
    return TEQ_M;
  }
  if (strcmp(str, "cmp")) {
    return CMP_M;
  }
  if (strcmp(str, "mul")) {
    return MUL_M;
  }
  if (strcmp(str, "mla")) {
    return MLA_M;
  }
  if (strcmp(str, "ldr")) {
    return LDR_M;
  }
  if (strcmp(str, "str")) {
    return STR_M;
  }
  if (strcmp(str, "beq")) {
    return BEQ_M;
  }
  if (strcmp(str, "bne")) {
    return BNE_M;
  }
  if (strcmp(str, "bge")) {
    return BGE_M;
  }
  if (strcmp(str, "blt")) {
    return BLT_M;
  }
  if (strcmp(str, "bgt")) {
    return BGT_M;
  }
  if (strcmp(str, "ble")) {
    return BLE_M;
  }
  if (strcmp(str, "b")) {
    return B_M;
  }
  if (strcmp(str, "lsl")) {
    return LSL_M;
  }
  if (strcmp(str, "andeq")) {
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
