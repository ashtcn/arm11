#include "decode.h"
#include "execute.h"
#include "print_compliant.h"

#define run_test(fn_name) \
  printf("Running tests: %-20s ", #fn_name); \
  fn_name(); \
  printf("Passed!\n");

void test_load_file(void) {
  byte_t load_file_memory[NUM_ADDRESSES] = {0};

  load_file("unit_tests_res/load_file_fname", load_file_memory);

  assert(0x01 == load_file_memory[0]);
  assert(0x10 == load_file_memory[1]);
  assert(0xa0 == load_file_memory[2]);
  assert(0xe3 == load_file_memory[3]);
  assert(0x02 == load_file_memory[4]);
  assert(0x20 == load_file_memory[5]);
  assert(0xa0 == load_file_memory[6]);
  assert(0xe3 == load_file_memory[7]);
  assert(0x02 == load_file_memory[8]);
  assert(0x00 == load_file_memory[9]);
  assert(0x11 == load_file_memory[10]);
  assert(0xe1 == load_file_memory[11]);
  assert(0x00 == load_file_memory[12]);
  assert(0x00 == load_file_memory[13]);
  assert(0x00 == load_file_memory[14]);
  assert(0x0a == load_file_memory[15]);
  assert(0x03 == load_file_memory[16]);
  assert(0x30 == load_file_memory[17]);
  assert(0xa0 == load_file_memory[18]);
  assert(0xe3 == load_file_memory[19]);
  assert(0x04 == load_file_memory[20]);
  assert(0x40 == load_file_memory[21]);
  assert(0xa0 == load_file_memory[22]);
  assert(0xe3 == load_file_memory[23]);
  for (uint32_t i = 24; i < NUM_ADDRESSES; i++) {
    assert(0x00 == load_file_memory[i]);
  }
}

void test_print_system_state(void) {
  instruction_t pss_instruction = {
    .type = DPI,
    .cond = GE,
    .operation = ORR,
    .immediate_value = 0xabc,
    .rn = 5,
    .rd = 6,
    .flag_0 = true,
    .flag_1 = true,
  };
  system_state_t pss_state = {
    .registers = {0},
    .memory = {0},
    .fetched_instruction = 0,
    .decoded_instruction = &pss_instruction,
    .has_fetched_instruction = false,
  };
  pss_state.registers[3] = 0xabcd0123;
  pss_state.registers[16] = 0x0123abcd;
  pss_state.memory[0] = 0xab;
  pss_state.memory[3] = 0x12;
  pss_state.memory[4] = 0x12;
  pss_state.memory[65535] = 0xab;
  print_system_state(&pss_state);
}

void test_shifter_values(word_t correct_value, bool correct_carry, value_carry_t *shifter_out) {
  assert(shifter_out->value == correct_value);
  assert(shifter_out->carry == correct_carry);
}

void test_shifter (void) {
  test_shifter_values(0x0F000000, false, shifter(LSL, 4, 0x00F00000));
  test_shifter_values(0xE1E00000, true, shifter(LSL, 1, 0xF0F00000));
  test_shifter_values(0x00000000, false, shifter(LSL, 32, 0x00100000));
  test_shifter_values(0x00000000, false, shifter(LSL, 33, 0x00000001));

  test_shifter_values(0x00F00000, false, shifter(LSR, 4, 0x0F000000));
  test_shifter_values(0x07800001, true, shifter(LSR, 1, 0x0F000003));
  test_shifter_values(0x00000000, true, shifter(LSR, 32, 0x80000000));

  test_shifter_values(0xFFF00000, false, shifter(ASR, 4, 0xFF000000));
  test_shifter_values(0xC7800001, true, shifter(ASR, 1, 0x8F000003));
  test_shifter_values(0xFFFFFFFF, true, shifter(ASR, 32, 0x8F000003));

  test_shifter_values(0x80000000, true, shifter(ROR, 1, 0x00000001));
  test_shifter_values(0x0F000000, false, shifter(ROR, 4, 0xF0000000));
  test_shifter_values(0x11111111, false, shifter(ROR, 4, 0x11111111));
  test_shifter_values(0xF0F00001, true, shifter(ROR, 4, 0x0F00001F));
  test_shifter_values(0x0F00001F, false, shifter(ROR, 32, 0x0F00001F));
  test_shifter_values(0xF0F00001, true, shifter(ROR, 36, 0x0F00001F));
  test_shifter_values(0x80000000, true, shifter(ROR, 64, 0x80000000));
}

static const instruction_t NULL_INSTRUCTION = {
  .type = NUL,
  .cond = AL,
  .operation = AND,
  .immediate_value = 0,
  .rn = -1,
  .rd = -1,
  .rs = -1,
  .rm = -1,
  .flag_0 = false,
  .flag_1 = false,
  .flag_2 = false,
  .flag_3 = false,
  .shift_type = ROR,
  .shift_amount = 0,
};

int equal_instruction(instruction_t i1, instruction_t i2) {
  return i1.type == i2.type
         && i1.cond == i2.cond
         && i1.operation == i2.operation
         && i1.immediate_value == i2.immediate_value
         && i1.rn == i2.rn
         && i1.rd == i2.rd
         && i1.rs == i2.rs
         && i1.rm == i2.rm
         && i1.flag_0 == i2.flag_0
         && i1.flag_1 == i2.flag_1
         && i1.flag_2 == i2.flag_2
         && i1.flag_3 == i2.flag_3
         && i1.shift_type == i2.shift_type
         && i1.shift_amount == i2.shift_amount;
}

void test_decode_dpi(void) {
  system_state_t *fetch1 = malloc(sizeof(system_state_t));
  system_state_t fetch1_struct = {
    .registers = {0},
    .memory = {0},
    // AL      I TST  S Rn1  Rd2  ROR2 Imm
    // 1110 00 1 1000 1 0001 0010 0010 01010101
    .fetched_instruction = 0xE3112255,
    .has_fetched_instruction = true,
  };
  *fetch1 = fetch1_struct;
  fetch1->decoded_instruction = malloc(sizeof(instruction_t));
  *fetch1->decoded_instruction = NULL_INSTRUCTION;
  instruction_t decode1 = {
    .type = DPI,
    .cond = AL,
    .operation = TST,
    .immediate_value = 0x55,
    .rn = 1,
    .rd = 2,
    .rs = -1,
    .rm = -1,
    .flag_0 = true,
    .flag_1 = true,
    .flag_2 = false,
    .flag_3 = false,
    .shift_type = ROR,
    .shift_amount = 4,
  };
  system_state_t *fetch2 = malloc(sizeof(system_state_t));
  system_state_t fetch2_struct = {
    .registers = {0},
    .memory = {0},
    // EQ      I AND  S Rn0  Rd4  LSL4  LSL  Rm7
    // 0000 00 0 0000 1 0000 0100 00100 00 0 0111
    .fetched_instruction = 0x104207,
    .has_fetched_instruction = true,
  };
  *fetch2 = fetch2_struct;
  fetch2->decoded_instruction = malloc(sizeof(instruction_t));
  *fetch2->decoded_instruction = NULL_INSTRUCTION;
  instruction_t decode2 = {
    .type = DPI,
    .cond = EQ,
    .operation = AND,
    .immediate_value = 0,
    .rn = 0,
    .rd = 4,
    .rs = -1,
    .rm = 7,
    .flag_0 = false,
    .flag_1 = true,
    .flag_2 = false,
    .flag_3 = false,
    .shift_type = LSL,
    .shift_amount = 4,
  };
  system_state_t *fetch3 = malloc(sizeof(system_state_t));
  system_state_t fetch3_struct = {
    .registers = {0},
    .memory = {0},
    // GE      I MOV  S Rn10 Rd8  Rs3    ASR  Rm5
    // 1010 00 0 1101 0 1010 1000 0011 0 10 1 0101
    .fetched_instruction = 0xA1AA8355,
    .has_fetched_instruction = true,
  };
  *fetch3 = fetch3_struct;
  fetch3->decoded_instruction = malloc(sizeof(instruction_t));
  *fetch3->decoded_instruction = NULL_INSTRUCTION;
  instruction_t decode3 = {
    .type = DPI,
    .cond = GE,
    .operation = MOV,
    .immediate_value = 0,
    .rn = 10,
    .rd = 8,
    .rs = 3,
    .rm = 5,
    .flag_0 = false,
    .flag_1 = false,
    .flag_2 = false,
    .flag_3 = false,
    .shift_type = ASR,
    .shift_amount = 0,
  };
  decode_instruction(fetch1);
  decode_instruction(fetch2);
  decode_instruction(fetch3);
  assert (equal_instruction(*fetch1->decoded_instruction, decode1));
  assert (equal_instruction(*fetch2->decoded_instruction, decode2));
  assert (equal_instruction(*fetch3->decoded_instruction, decode3));
  free(fetch1->decoded_instruction);
  free(fetch2->decoded_instruction);
  free(fetch3->decoded_instruction);
  free(fetch1);
  free(fetch2);
  free(fetch3);
}

void test_decode_mul(void) {
  system_state_t *fetch4 = malloc(sizeof(system_state_t));
  system_state_t fetch4_struct = {
    .registers = {0},
    .memory = {0},
    // AL          A S Rd0  Rn1  Rs2       Rm3
    // 1110 000000 1 1 0000 0001 0010 1001 0011
    .fetched_instruction = 0xE0301293,
    .has_fetched_instruction = true,
  };
  *fetch4 = fetch4_struct;
  fetch4->decoded_instruction = malloc(sizeof(instruction_t));
  *fetch4->decoded_instruction = NULL_INSTRUCTION;
  instruction_t decode4 = {
    .type = MUL,
    .cond = AL,
    .operation = AND,
    .immediate_value = 0,
    .rn = 1,
    .rd = 0,
    .rs = 2,
    .rm = 3,
    .flag_0 = true,
    .flag_1 = true,
    .flag_2 = false,
    .flag_3 = false,
    .shift_type = ROR,
    .shift_amount = 0,
  };
  system_state_t *fetch5 = malloc(sizeof(system_state_t));
  system_state_t fetch5_struct = {
    .registers = {0},
    .memory = {0},
    // AL          A S Rd12 Rn7  Rs6       Rm5
    // 1110 000000 0 0 1100 0111 0110 1001 0101
    .fetched_instruction = 0xE00C7695,
    .has_fetched_instruction = true,
  };
  *fetch5 = fetch5_struct;
  fetch5->decoded_instruction = malloc(sizeof(instruction_t));
  *fetch5->decoded_instruction = NULL_INSTRUCTION;
  instruction_t decode5 = {
    .type = MUL,
    .cond = AL,
    .operation = AND,
    .immediate_value = 0,
    .rn = 7,
    .rd = 12,
    .rs = 6,
    .rm = 5,
    .flag_0 = false,
    .flag_1 = false,
    .flag_2 = false,
    .flag_3 = false,
    .shift_type = ROR,
    .shift_amount = 0,
  };
  decode_instruction(fetch4);
  decode_instruction(fetch5);
  assert (equal_instruction(*fetch4->decoded_instruction, decode4));
  assert (equal_instruction(*fetch5->decoded_instruction, decode5));
  free(fetch4->decoded_instruction);
  free(fetch5->decoded_instruction);
  free(fetch4);
  free(fetch5);
}

void test_decode_sdt(void) {
  system_state_t *fetch6 = malloc(sizeof(system_state_t));
  system_state_t fetch6_struct = {
    .registers = {0},
    .memory = {0},
    // AL      I P U     L Rn0  Rd1  Rs3    ASR  Rm6
    // 1110 01 1 1 1 0 0 1 0000 0001 0011 0 10 1 0110
    .fetched_instruction = 0xE7901356,
    .has_fetched_instruction = true,
  };
  *fetch6 = fetch6_struct;
  fetch6->decoded_instruction = malloc(sizeof(instruction_t));
  *fetch6->decoded_instruction = NULL_INSTRUCTION;
  instruction_t decode6 = {
    .type = SDT,
    .cond = AL,
    .operation = AND,
    .immediate_value = 0,
    .rn = 0,
    .rd = 1,
    .rs = 3,
    .rm = 6,
    .flag_0 = true,
    .flag_1 = true,
    .flag_2 = true,
    .flag_3 = true,
    .shift_type = ASR,
    .shift_amount = 0,
  };
  system_state_t *fetch7 = malloc(sizeof(system_state_t));
  system_state_t fetch7_struct = {
    .registers = {0},
    .memory = {0},
    // AL      I P U     L Rn4  Rd5  Offset 0x555
    // 1110 01 0 0 0 0 0 0 0100 0101 010101010101
    .fetched_instruction = 0xE4045555,
    .has_fetched_instruction = true,
  };
  *fetch7 = fetch7_struct;
  fetch7->decoded_instruction = malloc(sizeof(instruction_t));
  *fetch7->decoded_instruction = NULL_INSTRUCTION;
  instruction_t decode7 = {
    .type = SDT,
    .cond = AL,
    .operation = AND,
    .immediate_value = 0x555,
    .rn = 4,
    .rd = 5,
    .rs = -1,
    .rm = -1,
    .flag_0 = false,
    .flag_1 = false,
    .flag_2 = false,
    .flag_3 = false,
    .shift_type = ROR,
    .shift_amount = 0,
  };
  decode_instruction(fetch6);
  decode_instruction(fetch7);
  assert (equal_instruction(*fetch6->decoded_instruction, decode6));
  assert (equal_instruction(*fetch7->decoded_instruction, decode7));
  free(fetch6->decoded_instruction);
  free(fetch7->decoded_instruction);
  free(fetch6);
  free(fetch7);
}

void test_decode_bra(void) {
  system_state_t *fetch8 = malloc(sizeof(system_state_t));
  system_state_t fetch8_struct = {
    .registers = {0},
    .memory = {0},
    // GE        Offset 0x155554 (01 0101 0101 0101 0101 0101 0100)
    // 1010 1010 010101010101010101010101
    .fetched_instruction = 0xAA555555,
    .has_fetched_instruction = true,
  };
  *fetch8 = fetch8_struct;
  fetch8->decoded_instruction = malloc(sizeof(instruction_t));
  *fetch8->decoded_instruction = NULL_INSTRUCTION;
  instruction_t decode8 = {
    .type = BRA,
    .cond = GE,
    .operation = AND,
    .immediate_value = 0x1555554,
    .rn = -1,
    .rd = -1,
    .rs = -1,
    .rm = -1,
    .flag_0 = false,
    .flag_1 = false,
    .flag_2 = false,
    .flag_3 = false,
    .shift_type = ROR,
    .shift_amount = 0,
  };
  decode_instruction(fetch8);
  assert (equal_instruction(*fetch8->decoded_instruction, decode8));
  free(fetch8->decoded_instruction);
  free(fetch8);
}

int main(void) {
  run_test(test_load_file);
  // run_test(test_print_system_state); // Requires manual checks
  run_test(test_shifter);
  run_test(test_decode_dpi);
  run_test(test_decode_mul);
  run_test(test_decode_sdt);
  run_test(test_decode_bra);
  printf("\nNo errors\n");
  return 0;
}
