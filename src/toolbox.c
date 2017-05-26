#include <stdio.h>

void print_array(void *p, unsigned int bytes_to_print) {
  unsigned char *memory = (unsigned char *) p;
  for (size_t i = 0; i < bytes_to_print; i++) {
    printf("%i", memory[i]);

    // New line at every 4 bytes
    if (i % 16 == 15) {
      printf("\n");
    }
  }
  printf("\n");
}

unsigned long get_word(unsigned char *memory, unsigned int address) {
  unsigned long word = 0;
  for (size_t i = 3; i >= 0; i--) {
    word = ((unsigned long) memory[address + i * 4]) | (word << 4);
  }
  return word;
}
