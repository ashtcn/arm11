#include <assert.h>
#include "toolbox.h"

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
    .cond = 3,
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

int main(void) {
  run_test(test_load_file);
  run_test(test_print_system_state);
  printf("\nNo errors\n");
  return 0;
}
