#include "assemble_toolbox.h"

void save_file(word_t *data, char *file_name, int file_size) {
  FILE *file = fopen(file_name, "wb");
  if (file == NULL) {
    perror("Error in opening save file.");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < file_size; i++) {
    fwrite(&data[i], sizeof(word_t), 1, file);
  }

  fclose(file);
}

int lines_in_file(char *file_name) {
  int lines = 0;
  char ch;
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    perror("Error in opening source file.");
    exit(EXIT_FAILURE);
  }
  bool on_new_line = true;
  while((ch=getc(file)) != EOF) {
    if(ch == '\n') {
      if(!on_new_line) {
        lines++;
      }
      on_new_line = true;
    } else {
      on_new_line = false;
    }
  }
  fclose(file);
  return lines;
}

char **create_2d_array(unsigned int rows, unsigned int cols) {
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

void free_2d_array(char** arr) {
  free(arr[0]);
  free(arr);
}

char **load_source_file(char *load_filename, int lines) {
  int max_line_length = 512; //put in global
  FILE *file = fopen(load_filename, "r");
  if (file == NULL) {
    perror("Error in opening source file.");
    exit(EXIT_FAILURE);
  }

  //Set up the correct size array;
  char **loaded_file = create_2d_array(lines, max_line_length);

  // Try to read the file line by line
  int size = 0;
  while(size < lines && fgets(loaded_file[size], sizeof(char[max_line_length]), file)) {
    // Strips any trailing newlinesword_size
    if(loaded_file[size][0] != '\n' && loaded_file[size][0] != '\r') {
      loaded_file[size][strcspn(loaded_file[size], "\n")] = 0;
      size++;
    }
	}
  fclose(file);
  return loaded_file;
}

bool is_label(char* instruction) {
  return instruction[strlen(instruction) - 1] == ':';
}

char *trim(char *string) {
  // Remove leading whitespace
  while(' ' == *string) {
    string++;
  }
  return string;
}

string_array_t *tokenize_instruction(char* instruction) {
  string_array_t *result = (string_array_t *) malloc(sizeof(string_array_t));
  if(!result) {
    perror("Error allocating memory for tokens.");
    exit(EXIT_FAILURE);
  }

  if(is_label(instruction)) {
    result->size = 1;
    result->array = create_2d_array(1, 32);
    int label_length = strlen(instruction);
    strncpy(result->array[0], instruction, label_length - 1);
    result->array[0][label_length - 1] = '\0';
  } else {

    int space_count = 0;
    int i;
    for (i=0; instruction[i]; i++) {
      if(instruction[i] == ' ') {
        space_count++;
      }
    }

    if(space_count == 0) {
      result->size = 1;
      result->array = create_2d_array(1, 32);
      result->array[0] = instruction;
    } else {
      char *instruction_op = NULL;
      char *operands = strdup(instruction);
      instruction_op = strtok_r(operands, " ", &operands);
      result = tokenize_operand_instruction(result, trim(instruction_op), trim(operands));
      free(operands);
    }

  }
  return result;
}

string_array_t *tokenize_operand_instruction(string_array_t *result, char* instruction_op, char* operands) {
  int split_count = 0;
  int i;
  for (i=0; operands[i]; i++) {
    if(operands[i] == ','
    || operands[i] == ']'
    || operands[i] == '['
    || operands[i] == ' ') {
      split_count++;
    }
    if (operands[i] == ',' && operands[i+1] && operands[i+1] == ' ') {
      split_count--;
    }
  }
  result->size = split_count + 2;
  result->array = create_2d_array(split_count + 2, 32);

  (result->array)[0] = instruction_op;
  i = 0;
  int cur_section = 1;
  int start_split = 0;
  for (i=0; operands[i]; i++) {
    if(operands[i] == ',' && operands[i+1] && operands[i+1] == ' ') {
      strncpy(result->array[cur_section], &operands[start_split], i - start_split);
      cur_section++;
      start_split = i + 2;
      i++;
    } else if((operands[i] == ',' && !(operands[i+1] && operands[i+1] == ' '))) {
      strncpy(result->array[cur_section], &operands[start_split], i - start_split);
      cur_section++;
      start_split = i + 1;
    } else if(operands[i] == ' ') {
      strncpy(result->array[cur_section], &operands[start_split], i - start_split);
      cur_section++;
      start_split = i + 1;
    } else if(operands[i] == ']') {
      strncpy(result->array[cur_section], &operands[start_split], i - start_split);
      cur_section++;
      start_split = i;
    } else if(operands[i] == '[') {
      strncpy(result->array[cur_section], &operands[start_split], i - start_split + 1);
      cur_section++;
      start_split = i + 1;
    }
  }
  strncpy(result->array[cur_section], &operands[start_split], i - start_split + 1);

  return result;
}
