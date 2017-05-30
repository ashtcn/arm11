#include <stdlib.h>
#include <stdio.h>
#include "global.h"
#include "instruction.h"
#include "system_state.h"
#include "toolbox.h"
#include "execute.c"

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

void decode_instruction(system_state_t *machine) {
  // PRE: Instruction is not all 0
  word_t instruction = machine->fetched_instruction;
  machine->decoded_instruction->cond = (instruction & ~(MASK_FIRST_4)) >> (WORD_SIZE - 4);
  instruction &= MASK_FIRST_4; // Remove cond
  if ((instruction >> (WORD_SIZE - 8)) == 0xA) { // 0xA = 1010
    branch(machine);
  } else if ((instruction >> (WORD_SIZE - 6)) == 0x1) { // Single Data Transfer
    single_data_transfer(instruction & MASK_FIRST_6, operation);
  } else if (!(instruction >> 22) && (((instruction >> 4) & 0xF) == 0x9)) {
    //Multiply
    multiply(instruction, operation);
  } else if (!(instruction >> (WORD_SIZE - 6))) { // Dataprocessing
    data_processing(instruction, operation);
  } else {
    fprintf(stderr, "Unknown instruction, PC: %u", machine->registers[PC]);
    exit_program(machine);
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

  *machine = DEFAULT_SYSTEM_STATE;
  machine->decoded_instruction = malloc(sizeof(instruction_t));
  *(machine->decoded_instruction) = NULL_INSTRUCTION;
  load_file(filename, machine->memory);

  while (machine->decoded_instruction->type != ZER) {
    // Execute
    if (machine->decoded_instruction->type != NUL) {
      execute(machine);
    }

    // Decode
    *(machine->decoded_instruction) = NULL_INSTRUCTION;
    if (machine->has_fetched_instruction) {
      decode_instruction(machine);
    }

    // Fetch
    if (machine->decoded_instruction->type != ZER) {
      machine->fetched_instruction = get_word(machine, machine->registers[PC]);
    }

    machine->registers[PC] += 4;
  }

  print_system_state(machine);
  return EXIT_SUCCESS;
}
