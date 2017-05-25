#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "toolbox.c"

unsigned char test_memory[NUM_ADDRESSES];

void load_file(char *fname, unsigned char memory[]) {
  FILE *file;

  file = fopen(fname,"rb");
  fread(memory, NUM_ADDRESSES, 1, file);
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
