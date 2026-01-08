#ifndef UTILS_H
#define UTILS_H
#define BUFFER_SIZE 256
#define ALFABET_SIZE 130
#include "structs.h"
#include <stdio.h>

void get_command_name(char cmd[], char **cmd_name);
void read_line(char *line, FILE *stream);
ProgramState init_state();
void pop(StackNode **stack);
void clear_stack(StackNode **stack);
void push(StackNode **stack, ProgramState state);
ProgramState get_head(StackNode *stack);
ProgramState state_dup(ProgramState state);
LSystem sys_dup(LSystem lsys);
void free_lsys(LSystem sys);

#endif
