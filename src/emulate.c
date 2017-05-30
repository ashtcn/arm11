#include <stdlib.h>
#include <stdio.h>
#include "global.h"
#include "instruction.h"
#include "system_state.h"
#include "toolbox.h"

void load_file(char *fname, byte *memory) {
  FILE *file;
  file = fopen(fname,"rb");
  fread(memory, NUM_ADDRESSES, 1, file);
}

void decode_instruction(word fetched_instr, instruction *operation) {
  // PRE: Instruction is not all 0
  operation->cond = (fetched_instr & ~(MASK_FIRST_4)) >> (WORD_SIZE - 4);
  fetched_instr &= MASK_FIRST_4; // Remove cond
  if ((fetched_instr >> (WORD_SIZE - 8)) == 0xA) { // 0xA = 1010
    branch(fetched_instr & MASK_FIRST_8, operation);
  } else if ((fetched_instr >> (WORD_SIZE - 6)) == 0x1) {//Single Data Transfer
    single_data_transfer(fetched_instr & MASK_FIRST_6, operation);
  } else if (!(fetched_instr >> 22) && (((fetched_instr >> 4) & 0xF) == 0x9)) {
    //Multiply
    multiply(fetched_instr, operation);
  } else if (!(fetched_instr >> (WORD_SIZE - 6)) { // Dataprocessing
    data_processing(fetched_instr, operation);
  } else {
    fprintf(stderr, "Unknown instruction, PC: %u", registers[PC]); // How access registers
    exit_program();
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Incorrect number of arguments provided.\n");
    return EXIT_FAILURE;
  }

  char *filename = argv[1];
  printf("%s\n", filename);

  system_state machine;

  return EXIT_SUCCESS;
}
