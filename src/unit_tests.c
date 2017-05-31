#include "decode.h"
#include "execute.h"

#define run_test(fn_name) \
  printf("%s\n", #fn_name); \
  fn_name()

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

void test_print_system_state() {
  instruction_t pss_instruction = {
    .type = DPI,
    .cond = ge,
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
  test_shifter_values(0x0F000000, false, shifter(lsl, 4, 0x00F00000));
  test_shifter_values(0xE1E00000, true, shifter(lsl, 1, 0xF0F00000));
  test_shifter_values(0x00000000, false, shifter(lsl, 32, 0x00100000));
  test_shifter_values(0x00000000, false, shifter(lsl, 33, 0x00000001));

  test_shifter_values(0x00F00000, false, shifter(lsr, 4, 0x0F000000));
  test_shifter_values(0x07800001, true, shifter(lsr, 1, 0x0F000003));
  test_shifter_values(0x00000000, true, shifter(lsr, 32, 0x80000000));

  test_shifter_values(0xFFF00000, false, shifter(asr, 4, 0xFF000000));
  test_shifter_values(0xC7800001, true, shifter(asr, 1, 0x8F000003));
  test_shifter_values(0xFFFFFFFF, true, shifter(asr, 32, 0x8F000003));

  test_shifter_values(0x80000000, true, shifter(ror, 1, 0x00000001));
  test_shifter_values(0x0F000000, false, shifter(ror, 4, 0xF0000000));
  test_shifter_values(0x11111111, false, shifter(ror, 4, 0x11111111));
  test_shifter_values(0xF0F00001, true, shifter(ror, 4, 0x0F00001F));
  test_shifter_values(0x0F00001F, false, shifter(ror, 32, 0x0F00001F));
  test_shifter_values(0xF0F00001, true, shifter(ror, 36, 0x0F00001F));
  test_shifter_values(0x80000000, true, shifter(ror, 64, 0x80000000));
}




int main(void) {
  run_test(test_load_file);
  run_test(test_print_system_state); // Requires manual check
  run_test(test_shifter);
  printf("\nNo errors\n");
  return 0;
}
