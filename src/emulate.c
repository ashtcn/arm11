#include "instruction.h"
#include "decode.h"
#include "execute.h"

static const system_state_t DEFAULT_SYSTEM_STATE = {
  .registers = {0},
  .memory = {0},
  .fetched_instruction = 0,
  .has_fetched_instruction = false,
};

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
    print_system_state(machine);
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

    machine->registers[PC] += 4;
  }

  printf("\nProgram executed successfully\n");

  print_system_state(machine);

  free(machine->decoded_instruction);
  free(machine);

  return EXIT_SUCCESS;
}
