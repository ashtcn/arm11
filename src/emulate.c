#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "toolbox.c"
#include "system_state.c"

void load_file(char *fname, unsigned char *memory) {
  FILE *file;
  file = fopen(fname,"rb");
  fread(memory, NUM_ADDRESSES, 1, file);
}

void decode_instruction(unsigned long instruction) {
  // PRE: Instruction is not all 0
  instruction = instruction & 0xFFFFFFF; //Remove cond
  if ((instruction >> 24) == 0xA) {//Branch
    branch(instruction & 0xFFFFFF);
  } else if ((instruction >> 26) == 0x1) {//Single Data Transfer
    single_data_transfer(instruction & 0x3FFFFF);
  } else if (!(instruction >> 22) && (((instruction >> 4) & 0xF) == 0x9)) {
    //Multiply
    multiply(instruction);
  } else if (!(instruction >> 26) { // Dataprocessing
    data_processing(instruction);
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
