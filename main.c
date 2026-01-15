#include "fonts.h"
#include "image.h"
#include "lsystems.h"
#include "structs.h"
#include "turtle.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void perform_load(char *cmd, programstate *current_state,
				  stacknode **undo_stack, stacknode **redo_stack)
{
	if (strlen(cmd) < 6) {
		printf("Failed to load\n");
		return;
	}
	char *path_to_file = malloc(strlen(cmd));
	strcpy(path_to_file, cmd + 5);

	image new_img = load_image(path_to_file);
	if (new_img.w == -1) {
		// free(cmd_name);
		free(path_to_file);
		return;
	}

	printf("Loaded %s (PPM image %dx%d)\n", path_to_file, new_img.w, new_img.h);

	programstate new_state = state_dup((*current_state));
	free_img(new_state.img);
	new_state.img = img_dup(new_img);

	// Captarea outputului
	int len = snprintf(NULL, 0, "Loaded %s (PPM image %dx%d)\n", path_to_file,
					   new_img.w, new_img.h);
	free(new_state.last_output);
	new_state.last_output = malloc(len + 5);
	snprintf(new_state.last_output, len + 1, "Loaded %s (PPM image %dx%d)\n",
			 path_to_file, new_img.w, new_img.h);

	update_state(undo_stack, redo_stack, current_state, &new_state);
	free_img(new_img);
	free(path_to_file);
}

void perform_lsystem(char *cmd, programstate *current_state,
					 stacknode **undo_stack, stacknode **redo_stack)
{
	if (strlen(cmd) < 9) {
		printf("Failed to load %s\n", cmd + 7);
		return;
	}
	char *path_to_file = malloc(strlen(cmd));
	strcpy(path_to_file, cmd + 8);
	lsystem new_lsys = load_lsys(path_to_file);

	if (new_lsys.rules_count == -1)
		printf("Failed to load %s\n", path_to_file), new_lsys.axiom = NULL;

	else {
		printf("Loaded %s (L-system with %d rules)\n", path_to_file,
			   new_lsys.rules_count);

		programstate new_state = state_dup((*current_state));
		free_lsys(new_state.lsys);
		new_state.lsys = sys_dup(new_lsys);

		// Captarea outputului
		int len = snprintf(NULL, 0, "Loaded %s (L-system with %d rules)\n",
						   path_to_file, new_lsys.rules_count);
		free(new_state.last_output);
		new_state.last_output = malloc(len + 5);
		snprintf(new_state.last_output, len + 1,
				 "Loaded %s (L-system with %d rules)\n", path_to_file,
				 new_lsys.rules_count);

		update_state(undo_stack, redo_stack, current_state, &new_state);
		free_lsys(new_lsys);
	}
	free(path_to_file);
}

void perform_turtle(char *cmd, programstate *current_state,
					stacknode **undo_stack, stacknode **redo_stack)
{
	if (current_state->img.w == -1) {
		printf("No image loaded\n");
		return;
	}

	if (current_state->lsys.rules_count == -1) {
		printf("No L-system loaded\n");
		return;
	}

	turtle t;
	t.stack = NULL;
	get_turtle_args(&t, cmd + 7);
	char *final = calloc(BUFFER_SIZE, sizeof(char));
	char *init = strdup(current_state->lsys.axiom);
	deriv(&init, t.n, current_state->lsys.rules, &final);
	image new_img = execute_string(&t, final, current_state->img);
	free(t.stack);
	free(init);

	programstate new_state = state_dup((*current_state));
	free_img(new_state.img);
	new_state.img = img_dup(new_img);
	free(new_state.last_output);
	new_state.last_output = strdup("Drawing done\n");
	update_state(undo_stack, redo_stack, current_state, &new_state);
	free_img(new_img);

	printf("Drawing done\n");
}

void perform_derive(char *cmd, programstate *current_state)
{
	char *end;
	int n = strtol(cmd + 6, &end, 10);

	if (current_state->lsys.rules_count == -1) {
		printf("No L-system loaded\n");
		return;
	}
	char *final = calloc(BUFFER_SIZE, sizeof(char));
	char *init = strdup(current_state->lsys.axiom);
	deriv(&init, n, current_state->lsys.rules, &final);
	printf("%s\n", final);
	free(final);
	// free(init);
}

void perform_save(char *cmd, programstate *current_state)
{
	if (current_state->img.w == -1) {
		printf("No image loaded\n");
		return;
	}

	char *path_to_file = malloc(strlen(cmd));
	strcpy(path_to_file, cmd + 5);
	save_image(current_state->img, path_to_file);
	printf("Saved %s\n", path_to_file);
	free(path_to_file);
}

void perform_font(char *cmd, programstate *current_state,
				  stacknode **undo_stack, stacknode **redo_stack)
{
	char *path_to_file = strdup(cmd + 5);
	if (!path_to_file) {
		printf("Failed to allocate memory in func main\n");
		return;
	}

	char *name;
	int new_size;
	font *new_font = load_font(path_to_file, &name, &new_size);

	if (!new_font) {
		printf("Failed to load %s\n", path_to_file);
		free(path_to_file);
		return;
	}

	programstate new_state = state_dup((*current_state));
	free_font(new_state.fonts, new_state.fonts_size);
	new_state.fonts = font_dup(new_font, new_size);
	new_state.fonts_size = new_size;
	if (new_state.font_name)
		free(new_state.font_name);

	new_state.font_name = name;
	free_font(new_font, new_size);

	int len =
		snprintf(NULL, 0, "Loaded %s (bitmap font %s)\n", path_to_file, name);
	free(new_state.last_output);
	new_state.last_output = malloc(len + 5);
	snprintf(new_state.last_output, len + 1, "Loaded %s (bitmap font %s)\n",
			 path_to_file, new_state.font_name);

	update_state(undo_stack, redo_stack, current_state, &new_state);
	printf("Loaded %s (bitmap font %s)\n", path_to_file,
		   current_state->font_name);
	free(path_to_file);
}

void perform_type(char *cmd, programstate *current_state,
				  stacknode **undo_stack, stacknode **redo_stack)
{
	if (current_state->img.w == -1) {
		printf("No image loaded\n");
		return;
	}

	if (!current_state->fonts) {
		printf("No font loaded\n");
		return;
	}

	int start_x, start_y;
	char *text;
	pixel color;
	get_type_args(cmd, &text, &start_x, &start_y, &color);
	programstate new_state = state_dup((*current_state));
	type_text(text, start_x, start_y, &color, &new_state);

	int len = snprintf(NULL, 0, "Text written\n");
	free(new_state.last_output);
	new_state.last_output = malloc(len + 5);
	snprintf(new_state.last_output, len + 1, "Text written\n");

	update_state(undo_stack, redo_stack, current_state, &new_state);

	printf("Text written\n");
	free(text);
}

int main(void)
{
	programstate current_state = init_state();
	stacknode *undo_stack = NULL, *redo_stack = NULL;

	while (1) {
		char *cmd = NULL;
		read_line(&cmd, stdin);

		char *cmd_name;
		get_command_name(cmd, &cmd_name);

		if (!strcmp(cmd_name, "EXIT")) {
			free(cmd_name);
			free(cmd);
			free_state(&current_state);
			clear_stack(&undo_stack);
			clear_stack(&redo_stack);
			break;
		}

		else if (!strcmp(cmd_name, "UNDO")) {
			if (!undo_stack)
				printf("Nothing to undo\n");

			else {
				programstate prev_state = get_head(undo_stack);
				push(&redo_stack, state_dup(current_state));
				free_state(&current_state);
				current_state = state_dup(prev_state);
				pop(&undo_stack);
				// printf("%s", current_state.last_output);
			}
		}

		else if (!strcmp(cmd_name, "REDO")) {
			if (!redo_stack) {
				printf("Nothing to redo\n");
				free(cmd);
				free(cmd_name);
				continue;
			}

			programstate next_state = get_head(redo_stack);
			push(&undo_stack, state_dup(current_state));
			free_state(&current_state);
			current_state = state_dup(next_state);
			pop(&redo_stack);
			printf("%s", current_state.last_output);
		}

		else if (!strcmp(cmd_name, "LSYSTEM")) {
			perform_lsystem(cmd, &current_state, &undo_stack, &redo_stack);
		}

		else if (!strcmp(cmd_name, "DERIVE")) {
			perform_derive(cmd, &current_state);
		}

		else if (!strcmp(cmd_name, "LOAD")) {
			perform_load(cmd, &current_state, &undo_stack, &redo_stack);
		}

		else if (!strcmp(cmd_name, "SAVE")) {
			perform_save(cmd, &current_state);
		}

		else if (!strcmp(cmd_name, "TURTLE")) {
			perform_turtle(cmd, &current_state, &undo_stack, &redo_stack);
		}

		else if (!strcmp(cmd_name, "FONT"))
			perform_font(cmd, &current_state, &undo_stack, &redo_stack);

		else if (!strcmp(cmd_name, "TYPE")) {
			perform_type(cmd, &current_state, &undo_stack, &redo_stack);
		}

		free(cmd_name);
		free(cmd);
	}
	return 0;
}
