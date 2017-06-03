/**
 * @file emulate.c
 * @brief The main functionality for the ARM11 emulator.
 */

#include "emulate_utils/decode.h"
#include "emulate_utils/execute.h"
#include "emulate_utils/print_compliant.h"

/** A 0-initialised system state. */
static const system_state_t DEFAULT_SYSTEM_STATE = {
  .registers = {0},
  .memory = {0},
  .fetched_instruction = 0,
  .has_fetched_instruction = false,
};

/** A null (non-existant) instruction. */
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

/**
 * @brief Emulates an ARM11 machine operating on a given binary file.
 *
 * The user must provide a single argument, which is a valid file name for an
 * ARM11 binary object code file. This function emulates the ARM architecture,
 * returning details of the registers and non-zero memory at the end of
 * execution.
 */
int main(int argc, char **argv) {
  // Check for correct program arguments
  if (argc != 2) {
    fprintf(stderr, "Incorrect number of arguments provided.\n");
    return EXIT_FAILURE;
  }

  char *filename = argv[1];
  system_state_t *machine = malloc(sizeof(system_state_t));

  // Check if we cannot allocate memory
  if (!machine) {
    perror("Cannot allocate memory to store system_state.\n");
    return EXIT_FAILURE;
  }

  // Set up a 0-initialised system state and load the program
  *machine = DEFAULT_SYSTEM_STATE;
  machine->decoded_instruction = malloc(sizeof(instruction_t));
  *(machine->decoded_instruction) = NULL_INSTRUCTION;
  load_file(filename, machine->memory);

  // The main execution loop of the emulator
  while (machine->decoded_instruction->type != ZER) {
    // Print details for current cycle if not in COMPLIANT_MODE
    if (!COMPLIANT_MODE) {
      print_system_state(machine);
    }

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
      machine->has_fetched_instruction = true;
    } else {
      machine->has_fetched_instruction = false;
    }

    // Next instruction
    machine->registers[PC] += 4;
  }

  // Print out final details
  if(COMPLIANT_MODE) {
    print_system_state_compliant(machine);
  } else {
    printf("\nProgram executed successfully\n");
    print_system_state(machine);
  }

  free(machine->decoded_instruction);
  free(machine);

  return EXIT_SUCCESS;
}
