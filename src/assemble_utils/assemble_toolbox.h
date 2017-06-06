#ifndef ASSEMBLE_TOOLBOX_H
#define ASSEMBLE_TOOLBOX_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../global.h"
#include "string_array.h"
#include "string_arrays.h"
#include <stdbool.h>

void save_file(word_t *data, char *file_name, int ile_size);

int lines_in_file(char *file_name);
char **load_source_file(char *load_filename, int lines);
char **create_2d_array(unsigned int rows, unsigned int cols);
void free_2d_array(char ** arr, int rows);

#endif
