#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Incorrect number of arguments provided.\n");
    return EXIT_FAILURE;
  }

  char *filename = argv[1];
  printf("%s\n", filename);


  return EXIT_SUCCESS;
}
