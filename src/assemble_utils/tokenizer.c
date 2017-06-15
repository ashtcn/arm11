/**
 * @file tokenizer.c
 * @brief File for functions to tokenize assembly input.
 */

#include "tokenizer.h"

static bool is_label(char *instruction);
static char *trim(char *string);
static char *trim_token(char *string);
static char *split_token(string_array_t *result, int *cur_section, char *operands, int i);

static string_array_t *tokenize_operand_instruction(string_array_t *result, char *instruction_op, char *operands);
static string_array_t *tokenize_instruction(char *instruction);

/**
 * @brief Returns true iff the provided instruction is a label.
 *
 * An instruction is a label iff its final character is a colon.
 * @param instruction The instruction string to check.
 * @returns True iff the provided instruction is a label.
 */
static bool is_label(char *instruction) {
  return instruction[strlen(instruction) - 1] == ':';
}

/**
 * @brief Removes surrounding whitespace.
 *
 * Removes leading and trailing whitespace.
 * @param string The string to trim.
 * @returns The string with surrounding whitespace removed.
 */
static char *trim(char *string) {
  // Remove leading whitespace.
  while (' ' == *string) {
    string++;
  }

  // Remove trailing whitespace.
  int new_end = strlen(string);
  while (string[new_end] == ' ') {
    new_end--;
  }
  string[new_end] = '\0';
  return string;
}

/**
 * @brief Removes leading whitespace and commas.
 *
 * @param string The string to trim.
 * @returns The string with leading whitespace and commas removed.
 */
static char *trim_token(char *string) {
  while (' ' == *string || ',' == *string) {
    string++;
  }
  return string;
}

/**
 * @brief Tokenizes input into an array of string arrays.
 *
 * Tokenizes all instructions from the input, using tokenize_instruction.
 * @param input An array of strings for each line of input.
 * @param input_lines The number of input lines.
 * @returns String arrays, one for each instruction.
 */
string_arrays_t *tokenize_input(char **input, int input_lines) {
  string_arrays_t *result = make_string_arrays();

  for (int i = 0; i < input_lines; i++) {
    if (input[i][0] != '\n') {
      string_array_t *tokened = tokenize_instruction(input[i]);
      add_string_arrays(result, tokened);
    }
  }

  return result;
}

/**
 * @brief Tokenizes a single instruction into a string array.
 *
 * Breaks up an instruction into single pieces of information (seperated by
 * commas, spaces, brackets).
 * @param instruction The instructions string.
 * @returns A string array of the tokenized instruction.
 */
static string_array_t *tokenize_instruction(char *instruction) {
  string_array_t *result = malloc(sizeof(string_array_t));
  if (!result) {
    perror("Error allocating memory for tokens.");
    exit(EXIT_FAILURE);
  }

  if (is_label(instruction)) {
    result->size = 1;
    result->array = create_2d_array(1, 32);
    free(result->array[0]);
    result->array[0] = instruction;
    result->array[0][strlen(instruction) - 1] = '\0';
  } else {
    int space_count = 0;
    for (int i = 0; instruction[i]; i++) {
      if (instruction[i] == ' ') {
        space_count++;
      }
    }

    char *instruction_op = strtok_r(instruction, " ", &instruction);
    result = tokenize_operand_instruction(result, trim(instruction_op),
                                          trim(instruction));
  }
  return result;
}

static char *split_token(string_array_t *result, int *cur_section, char *operands,
                  int i) {
  strncpy(result->array[*cur_section], operands, i);
  result->array[*cur_section][i] = '\0';
  (*cur_section)++;
  return trim_token(operands + i);
}

/**
 * @brief A helper function for tokenize_instruction.
 *
 * Tokenizes operands.
 * @param result A pointer the the string array to write to.
 * @param instruction_op The operation of the instruction.
 * @param operands The operands of the instruction.
 * @returns The updated string array.
 */
static string_array_t *tokenize_operand_instruction(string_array_t *result,
                                             char *instruction_op,
                                             char *operands) {
  int split_count = 0;
  int i;
  for (i=0; operands[i]; i++) {
    // Count number of splits needed, ignoring duplicate or unecessary spaces
    if ((operands[i] == ','
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
    if (operands[i] == ',' || operands[i] == ' ') {
      // Cut out the comma or space and add the token.
      operands = split_token(result, &cur_section, operands, i);
      i = 0;
    } else if (operands[i] == ']') {
      // Add the token and start ahead of the bracket.
      operands = split_token(result, &cur_section, operands, i);
      i = 1;
    } else if (operands[i] == '[') {
      // Add the bracket token.
      operands = split_token(result, &cur_section, operands, i + 1);
      i = 0;
    } else {
      i++;
    }
  }

  // If there is a remaining token add it.
  if (i > 0) {
    strncpy(result->array[cur_section], operands, i);
    result->array[cur_section][i] = '\0';
  }

  return result;
}
