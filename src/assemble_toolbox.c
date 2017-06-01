#include "assemble_toolbox.h"

void save_file(word_t *data, char *file_name, int ile_size) {
//TODO
}

int lines_in_file(char *file_name) {
  int lines = 0;
  char ch;
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    perror("Error in opening source file.");
    exit(EXIT_FAILURE);
  }

  while((ch=getc(file)) != EOF) {
    if(ch == '\n') {
      ++lines;
    }
  }
  fclose(file);
  return lines;
}

char **create_file_array(unsigned int rows, unsigned int cols) {
  unsigned int i;
  char **loaded_file = (char **) malloc(rows * sizeof(char *));
  if(!loaded_file) {
    perror("Error allocating memory for source file.");
    exit(EXIT_FAILURE);
  }
  loaded_file[0] = (char *) malloc (cols * rows * sizeof(char));

  for(i = 1; i < rows; i++) {
    loaded_file[i] = loaded_file[i-1] + cols;
  }
  return loaded_file;
}

char **load_source_file(char *load_filename, int lines) {
  int max_line_length = 512; //put in global
  FILE *file = fopen(load_filename, "r");
  if (file == NULL) {
    perror("Error in opening source file.");
    exit(EXIT_FAILURE);
  }

  //Set up the correct size array;
  char **loaded_file = create_file_array(lines, max_line_length);

  // Try to read the file line by line
  int size = 0;
  while(fgets(loaded_file[size], sizeof(char[max_line_length]), file)) {
    // Strips any trailing newlinesword_size
    loaded_file[size][strcspn(loaded_file[size], "\n")] = 0;
    size++;
	}
  fclose(file);
  return loaded_file;
}
