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

void free_tokens(string_array_array_t *tokenized_input) {
  for (int i = 0; i < tokenized_input->size; i++) {
    for (int i2 = 1; i2 < tokenized_input->string_arrays[i]->size; i2++) {
      free(tokenized_input->string_arrays[i]->array[i2]);
    }
    free(tokenized_input->string_arrays[i]->array);
    free(tokenized_input->string_arrays[i]);
  }
  free(tokenized_input);
}


string_array_array_t *tokenize_input(char **input, int input_lines) {
  string_array_array_t *result = make_string_array_array();

  for (int i = 0; i < input_lines; i++) {
    if(input[i][0] != '\n') {
      string_array_t *tokened = tokenize_instruction(input[i]);
      add_string_array_array(result, tokened);
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
    int i;
    for (i=0; instruction[i]; i++) {
      if(instruction[i] == ' ') {
        space_count++;
      }
    }

    if(space_count == 0) {
      result->size = 1;
      result->array = create_2d_array(1, 32);
      free(result->array[0]);
      result->array[0] = instruction;
    } else {
      char *instruction_op = strtok_r(instruction, " ", &instruction);
      result = tokenize_operand_instruction(result, trim(instruction_op), trim(instruction));
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
  free(result->array[0]);
  result->array[0] = instruction_op;

  int cur_section = 1;
  int start_split = 0;
  for (i=0; operands[i]; i++) {
    if(operands[i] == ',' && operands[i+1] && operands[i+1] == ' ') {
      strncpy(result->array[cur_section], &operands[start_split], i - start_split);
      result->array[cur_section][i-start_split] = '\0';
      cur_section++;
      start_split = i + 2;
      i++;
    } else if(operands[i] == ',' || operands[i] == ' ') {
      strncpy(result->array[cur_section], &operands[start_split], i - start_split);
      result->array[cur_section][i-start_split] = '\0';
      cur_section++;
      start_split = i + 1;
    } else if(operands[i] == ']') {
      strncpy(result->array[cur_section], &operands[start_split], i - start_split);
      result->array[cur_section][i-start_split] = '\0';
      cur_section++;
      start_split = i;
    } else if(operands[i] == '[') {
      strncpy(result->array[cur_section], &operands[start_split], i - start_split + 1);
      result->array[cur_section][i-start_split + 1] = '\0';
      cur_section++;
      start_split = i + 1;
    }
  }
  strncpy(result->array[cur_section], &operands[start_split], i - start_split + 1);

  return result;
}
