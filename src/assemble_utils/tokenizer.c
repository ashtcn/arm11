#include "tokenizer.h"

bool is_label(char* instruction) {
  return instruction[strlen(instruction) - 1] == ':';
}

char *trim(char *string) {
  // Remove leading whitespace.
  while(' ' == *string) {
    string++;
  }

  // Remove trailing whitespace.
  int new_end = strlen(string);
  while(string[new_end] == ' ') {
    new_end--;
  }
  string[new_end] = '\0';
  return string;
}

char *trim_token(char *string) {
  // Remove leading whitespace and commas.
  while(' ' == *string || ',' == *string) {
    string++;
  }
  return string;
}

string_arrays_t *tokenize_input(char **input, int input_lines) {
  string_arrays_t *result = make_string_arrays();

  for (int i = 0; i < input_lines; i++) {
    if(input[i][0] != '\n') {
      string_array_t *tokened = tokenize_instruction(input[i]);
      add_string_arrays(result, tokened);
    }
  }

  return result;
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
    free(result->array[0]);
    result->array[0] = instruction;
    result->array[0][strlen(instruction) - 1] = '\0';
  } else {
    int space_count = 0;
    for (int i = 0; instruction[i]; i++) {
      if(instruction[i] == ' ') {
        space_count++;
      }
    }

    char *instruction_op = strtok_r(instruction, " ", &instruction);
    result = tokenize_operand_instruction(result, trim(instruction_op), trim(instruction));
  }
  return result;
}

char *split_token(string_array_t *result, int *cur_section, char* operands, int i) {
  strncpy(result->array[*cur_section], operands, i);
  result->array[*cur_section][i] = '\0';
  (*cur_section)++;
  return trim_token(operands + i);
}

string_array_t *tokenize_operand_instruction(string_array_t *result, char* instruction_op, char* operands) {
  int split_count = 0;
  int i;
  for (i=0; operands[i]; i++) {
    // Count number of splits needed, ignoring duplicate or unecessary spaces
    if((operands[i] == ','
     || operands[i] == ']'
     || operands[i] == '['
     || operands[i] == ' ') &&
     !(operands[i+1] && operands[i+1] == ' ')) {
      split_count++;
    }
  }
  result->size = split_count + 2;
  result->array = create_2d_array(split_count + 2, 32);
  free(result->array[0]);
  result->array[0] = instruction_op;

  int cur_section = 1;
  // Loop through the string, breaking on brackets, commas and spaces.
  for (i = 0; operands[i];) {
    if(operands[i] == ',' || operands[i] == ' ') {
      // Cut out the comma or space and add the token.
      operands = split_token(result, &cur_section, operands, i);
      i = 0;
    } else if(operands[i] == ']') {
      // Add the token and start ahead of the bracket.
      operands = split_token(result, &cur_section, operands, i);
      i = 1;
    } else if(operands[i] == '[') {
      // Add the bracket token.
      operands = split_token(result, &cur_section, operands, i + 1);
      i = 0;
    } else {
      i++;
    }
  }

  // If there is a remaining token add it.
  if(i > 0) {
    strncpy(result->array[cur_section], operands, i);
    result->array[cur_section][i] = '\0';
  }


  return result;
}
