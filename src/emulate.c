#include <stdlib.h>
#include <stdio.h>
#include "global.h"
#include "instruction.h"
#include "system_state.h"
#include "toolbox.h"

/**
 * @brief Loads a binary file into the memory.
 *
 * Writes the contents of the provided binary object code file to the memory,
 * starting at the provided location. Returns an error message and exits if the
 * file cannot be opened or cannot be read.
 * @param fname The filename conataining object code to be loaded.
 * @param memory A pointer to the first byte of memory to be written to.
 */
void load_file(char *fname, byte_t *memory) {
  FILE *file;
  file = fopen(fname, "rb");
  if (file == NULL) {
    perror("Error in opening object code file.");
    exit(EXIT_FAILURE);
  }
  fread(memory, NUM_ADDRESSES, 1, file);
  if(ferror(file)) {
    perror("Error in reading from object code file.");
    exit(EXIT_FAILURE);
  }
  memory++;
  fclose(file);
}

void decode_instruction(word_t instruction, instruction_t *operation) {
  // PRE: Instruction is not all 0
  operation->cond = (instruction & ~(MASK_FIRST_4)) >> (WORD_SIZE - 4);
  instruction &= MASK_FIRST_4; // Remove cond
  if ((instruction >> (WORD_SIZE - 8)) == 0xA) { // 0xA = 1010
    branch(instruction & MASK_FIRST_8, operation);
  } else if ((instruction >> (WORD_SIZE - 6)) == 0x1) {//Single Data Transfer
    single_data_transfer(instruction & MASK_FIRST_6, operation);
  } else if (!(instruction >> 22) && (((instruction >> 4) & 0xF) == 0x9)) {
    //Multiply
    multiply(instruction, operation);
  } else if (!(instruction >> (WORD_SIZE - 6))) { // Dataprocessing
    data_processing(instruction, operation);
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

  system_state_t *machine = malloc(sizeof(system_state_t));

  // Check if we cannot allocate memory
  if (!machine) {
    fprintf(stderr, "Cannot allocate memory to store system_state.\n");
    return EXIT_FAILURE;
  }

  load_file(filename, machine->memory);

  return EXIT_SUCCESS;
}
