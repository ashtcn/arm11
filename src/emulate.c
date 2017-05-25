#include <stdlib.h>
#include <stdio.h>
#include "constants.h"

unsigned char test_memory[NUM_ADDRESSES];

int load_file(char *fname, unsigned char memory[]) {
  FILE *file;

  file = fopen(fname,"rb");
  fread(memory, NUM_ADDRESSES, 1, file);

  return 1;
}

void print_array(unsigned char memory[], int bytes_to_print) {
  int i;
  for (i = 0; i < bytes_to_print; i++) {
    printf("%02x", memory[i]);
    if (i % 16 == 15) {
      printf("\n");
    }
  }
  printf("\n");
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
