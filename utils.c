#include "utils.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void get_command_name(char cmd[], char **cmd_name){
    *cmd_name = calloc(BUFFER_SIZE, sizeof(char));
    int cursor = 0;
    while(cursor < strlen(cmd) && isalpha(cmd[cursor]))
        (*cmd_name)[cursor] = cmd[cursor], cursor++;
}

void read_line(char *line, FILE *stream){
    // long cursor = ftell(stream);
    // while(1){
    //     fgets(line, sizeof(line), stream);
    //     if(line[strlen(line) - 1] != '\n'){
    //         fseek(stream, SEEK_SET, cursor);
    //         const int size = sizeof(line);
    //         free(line);
    //         line = malloc(size + BUFFER_SIZE);
    //     }
    //     else {
    //         line[strlen(line) - 1] = 0;
    //         break;
    //     }
    // }

    fgets(line, sizeof(line), stream);
    if(line[strlen(line) - 1] == '\n')
        line[strlen(line) - 1] = 0;
}


ProgramState init_state(){
    LSystem l;
    l.axiom = NULL;
    ProgramState state = {l};
    return state;
}
