#ifndef UTILS_H
#define UTILS_H
#define BUFFER_SIZE 256
#include "structs.h"
#include <stdio.h>

void get_command_name(char cmd[], char **cmd_name);
void read_line(char *line, FILE *stream);
ProgramState init_state();

#endif
