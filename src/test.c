#include <stdlib.h>
#include <stdbool.h>
//#include "instruction.h"
#include "constants.h"
#include "system_state.c"
#include "toolbox.c"

int main(int argc, char **argv) {
  system_state machine;
  print_system_state(machine);
  machine.registers[0] = 5;
  print_system_state(machine);
  machine.registers[1] = 3;
  return EXIT_SUCCESS;
}
