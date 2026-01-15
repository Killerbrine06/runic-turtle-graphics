#ifndef UTILS_H
#define UTILS_H
#define BUFFER_SIZE 256
#define ALFABET_SIZE 130
#include "structs.h"
#include <stdio.h>

void get_command_name(char cmd[], char **cmd_name);
void read_line(char **line, FILE *stream);
programstate init_state();
void pop(stacknode **stack);
void clear_stack(stacknode **stack);
void push(stacknode **stack, programstate state);
programstate get_head(stacknode *stack);
programstate state_dup(programstate state);
lsystem sys_dup(lsystem lsys);
image img_dup(image img);
font *font_dup(font *f, int size);
void free_lsys(lsystem sys);
void free_state(programstate *state);
void free_img(image img);
void free_font(font *f, int size);
void update_state(stacknode **undo_stack, stacknode **redo_stack,
				  programstate *current_state, programstate *new_state);
void get_turtle_args(turtle *t, char *cmd);
void get_type_args(char *cmd, char **text, int *start_x, int *start_y,
				   pixel *color);
char *_strnstr(char *big, char *little, int n);

#endif
