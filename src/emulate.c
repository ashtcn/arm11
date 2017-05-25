#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "opcode.h"
#include "toolbox.c"
#include "system_state.c"

unsigned char test_memory[NUM_ADDRESSES];

void exit_program();//Not made yet

void load_file(char *fname, unsigned char memory[]) {
  FILE *file;

  file = fopen(fname,"rb");
  fread(memory, NUM_ADDRESSES, 1, file);
}


int condition(unsigned long instruction) {
  instruction >>= 28;
  char flags = registers[CPSR] >>= 28;//how access registers
  const unsigned char V = 0x1;
  const unsigned char Z = 0x4;
  const unsigned char N = 0x8;
  switch(instruction) {
    case eq:
      return (flags & Z);
    case ne:
      return !(flags & Z);
    case ge:
      return (flags & V) == ((flags & N) >> 3);
    case lt:
      return (flags & V) != ((flags & N) >> 3);
    case gt:
      return !(flags & Z) && ((flags & V) == ((flags & N) >> 3));
    case le:
      return (flags & Z) || (flags & V) != ((flags & N) >> 3);
    case al:
      return 1;
    default:
      fprintf(stderr, "Incorrect cond flag, PC: %u", registers[PC]);//how access registers
      exit_program();//Not sure to exit program here
      return 0;
  }
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
  } else if (!(instruction >> 26) {//Dataprocessing
    data_processing(instruction);
  } else {
    fprintf(stderr, "Unknown instruction, PC: %u", registers[PC]);//how access registers
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

  print_array(test_memory, 16);
  load_file(filename, test_memory);
  print_array(test_memory, 16);

  return EXIT_SUCCESS;
}
