#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "structs.h"
#include "lsystems.h"
#include "image.h"
#include "turtle.h"

void perform_load(char *cmd, ProgramState *current_state, StackNode **undo_stack, StackNode **redo_stack){
    char *path_to_file = malloc(strlen(cmd));
    strcpy(path_to_file, cmd + 5);

    Image new_img = load_image(path_to_file);
    if(new_img.w == -1){
        // free(cmd_name);
        free(path_to_file);
        return;
    }

    printf("Loaded %s (PPM image %dx%d)\n", path_to_file, new_img.w, new_img.h);

    ProgramState new_state = state_dup((*current_state));
    free_img(new_state.img);
    new_state.img = img_dup(new_img);

    // Captarea outputului
    int len = snprintf(NULL, 0, "Loaded %s (PPM image %dx%d)\n", path_to_file, new_img.w, new_img.h);
    free(new_state.last_output);
    new_state.last_output = malloc(len + 5);
    snprintf(new_state.last_output, len + 1, "Loaded %s (PPM image %dx%d)\n", path_to_file, new_img.w, new_img.h);

    update_state(undo_stack, redo_stack, current_state, &new_state);
    free_img(new_img);
    free(path_to_file);
}

void perform_lsystem(char *cmd, ProgramState *current_state, StackNode **undo_stack, StackNode **redo_stack){
    char *path_to_file = malloc(strlen(cmd));
    strcpy(path_to_file, cmd + 8);
    LSystem new_lsys = load_lsys(path_to_file);

    if(new_lsys.rules_count == -1)
        printf("Failed to load %s\n", path_to_file), new_lsys.axiom = NULL;
    
    else {
        printf("Loaded %s (L-system with %d rules)\n", path_to_file, new_lsys.rules_count);

        ProgramState new_state = state_dup((*current_state));
        free_lsys(new_state.lsys);
        new_state.lsys = sys_dup(new_lsys);

        // Captarea outputului
        int len = snprintf(NULL, 0, "Loaded %s (L-system with %d rules)\n", path_to_file, new_lsys.rules_count);
        free(new_state.last_output);
        new_state.last_output = malloc(len + 5);
        snprintf(new_state.last_output, len + 1, "Loaded %s (L-system with %d rules)\n", path_to_file, new_lsys.rules_count);

        update_state(undo_stack, redo_stack, current_state, &new_state);
        free_lsys(new_lsys);
    }
    free(path_to_file);
}

void perform_turtle(char *cmd, ProgramState *current_state, StackNode **undo_stack, StackNode **redo_stack){
    if(current_state->img.w == -1){
        printf("No image loaded\n");
        return;
    }

    if(current_state->lsys.rules_count == -1){
        printf("No L-system loaded\n");
        return;
    }

    Turtle t;
    t.stack = NULL;
    get_turtle_args(&t, cmd + 7);
    char *end;
    char *final = calloc(BUFFER_SIZE, sizeof(char));
    char *init = strdup(current_state->lsys.axiom);
    deriv(&init, t.n, current_state->lsys.rules, &final);
    // printf("%d %s\n", t.n, final);
    Image new_img = execute_string(&t, final, current_state->img);
    free(t.stack);
    free(init);
    free(final);

    ProgramState new_state = state_dup((*current_state));
    free_img(new_state.img);
    new_state.img = img_dup(new_img);
    free(new_state.last_output);
    new_state.last_output = strdup("Drawing done\n");
    update_state(undo_stack, redo_stack, current_state, &new_state);
    free_img(new_img);

    printf("Drawing done\n");
}

int main(){
    char cmd[BUFFER_SIZE];
    ProgramState current_state = init_state();
    StackNode *undo_stack = NULL, *redo_stack = NULL;

    while(1){
        fgets(cmd, BUFFER_SIZE, stdin);
        if(cmd[strlen(cmd) - 1] == '\n')
            cmd[strlen(cmd) - 1] = 0;

        char *cmd_name;
        get_command_name(cmd, &cmd_name);

        if(!strcmp(cmd_name, "EXIT")){
            free(cmd_name);
            free_state(&current_state);
            clear_stack(&undo_stack);
            clear_stack(&redo_stack);
            break;
        }

        else if(!strcmp(cmd_name, "UNDO")){
            if(!undo_stack)
                printf("Nothing to undo\n");
            
            else {
                ProgramState prev_state = get_head(undo_stack);
                push(&redo_stack, state_dup(current_state));
                free_state(&current_state);
                current_state = state_dup(prev_state);
                pop(&undo_stack);
                // printf("%s", current_state.last_output);
            }
        }

        else if(!strcmp(cmd_name, "REDO")){
            if(!redo_stack){
                printf("Nothing to redo\n");
                free(cmd_name);
                continue;
            }

            ProgramState next_state = get_head(redo_stack);
            push(&undo_stack, state_dup(current_state));
            free_state(&current_state);
            current_state = state_dup(next_state);
            pop(&redo_stack);
            printf("%s", current_state.last_output);
        }
        
        else if(!strcmp(cmd_name, "LSYSTEM")){
            if(strlen(cmd) < 9){
                printf("Failed to load %s\n", cmd + 7);
                free(cmd_name);
                continue;
            }
            perform_lsystem(cmd, &current_state, &undo_stack, &redo_stack);
        }

        else if(!strcmp(cmd_name, "DERIVE")){
            char *end;
            int n = strtol(cmd + 6, &end, 10);

            if(current_state.lsys.rules_count == -1){
                printf("No L-system loaded\n");
                free(cmd_name);
                continue;
            }
            char *final = calloc(BUFFER_SIZE, sizeof(char));
            char *init = strdup(current_state.lsys.axiom);
            deriv(&init, n, current_state.lsys.rules, &final);
            printf("%s\n", final);
            free(final);
            free(init);
        }

        else if(!strcmp(cmd_name, "LOAD")){
            if(strlen(cmd) < 6){
                printf("Failed to load\n");
                free(cmd_name);
                continue;
            }

            perform_load(cmd, &current_state, &undo_stack, &redo_stack);
            // printf("%d %d %d\n", current_state.img.data[0][0].r, current_state.img.data[0][0].g, current_state.img.data[0][0].b);
        }

        else if(!strcmp(cmd_name, "SAVE")){
            if(current_state.img.w == -1){
                printf("No image loaded\n");
                free(cmd_name);
                continue;
            }

            char *path_to_file = malloc(strlen(cmd));
            strcpy(path_to_file, cmd + 5);
            save_image(current_state.img, path_to_file);
            printf("Saved %s\n", path_to_file);
            free(path_to_file);
        }
        
        else if(!strcmp(cmd_name, "TURTLE")){
            perform_turtle(cmd, &current_state, &undo_stack, &redo_stack);
        }

        free(cmd_name);
    }
    return 0;
}
