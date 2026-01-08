#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "structs.h"
#include "lsystems.h"

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
            break;
        }

        else if(!strcmp(cmd_name, "UNDO")){
            if(!undo_stack)
                printf("Nothing to undo\n");
            
            else {
                ProgramState prev_state = get_head(undo_stack);
                push(&redo_stack, current_state);
                free_lsys(current_state.lsys);
                current_state = state_dup(prev_state);
                pop(&undo_stack);
            }
        }

        else if(!strcmp(cmd_name, "LSYSTEM")){
            if(strlen(cmd) < 9)
                printf("Failed to load %s\n", cmd + 7);
            
            else{
                char *path_to_file = malloc(strlen(cmd));
                strcpy(path_to_file, cmd + 8);
                LSystem new_lsys = load_lsys(path_to_file);

                if(new_lsys.rules_count == -1)
                    printf("Failed to load %s\n", cmd + 7), new_lsys.axiom = NULL;
                
                else {
                    printf("Loaded %s (L-system with %d rules)\n", path_to_file, new_lsys.rules_count);
                    clear_stack(&redo_stack);
                    push(&undo_stack, state_dup(current_state));
                    free_lsys(current_state.lsys);
                    current_state.lsys = sys_dup(new_lsys);
                    free_lsys(new_lsys);
                }
                free(path_to_file);
            }
        }

        else if(!strcmp(cmd_name, "DERIV")){
            char *end;
            int n = strtol(cmd + 6, &end, 10);

            if(!current_state.lsys.axiom){
                printf("No L-system loaded\n");
                free(cmd_name);
                continue;
            }
            char *final = calloc(BUFFER_SIZE, sizeof(char));
            char *init = strdup(current_state.lsys.axiom);
            deriv(init, n, current_state.lsys.rules, &final);
            printf("%s\n", final);
            free(final);
            free(init);
        }

        free(cmd_name);
    }
    return 0;
}
