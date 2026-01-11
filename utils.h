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
Image img_dup(Image img);
void free_lsys(LSystem sys);
void free_state(ProgramState *state);
void free_img(Image img);
void update_state(StackNode **undo_stack, StackNode **redo_stack, ProgramState *current_state, ProgramState *new_state);
void get_turtle_args(Turtle *t, char *cmd);

#endif
