#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lsystems.h"
#include "utils.h"

LSystem load_lsys(char *path_to_file){
    LSystem sys;
    FILE *lsys_file = fopen(path_to_file, "r");
    
    if(!lsys_file){
        sys.rules_count = -1;
        return sys;
    }
    // sys.axiom = calloc(BUFFER_SIZE, 1);
    read_line(&sys.axiom, lsys_file);

    char *line = NULL, *end;
    read_line(&line, lsys_file);

    sys.rules_count = strtol(line, &end, 10);
    free(line);

    sys.rules = calloc(ALFABET_SIZE, sizeof(char*));

    for(int i=0; i<sys.rules_count; i++){
        // line = malloc(BUFFER_SIZE);
        read_line(&line, lsys_file);        
        sys.rules[line[0]] = malloc(strlen(line) + 1);
        strcpy(sys.rules[line[0]], line + 2);
        free(line);
    }

    fclose(lsys_file);

    return sys;
}

void deriv(char **init, int n, char **next, char **final){
    free((*final));
    if(!n){
        *final = strdup((*init));
        return;
    }
    while(n--){
        (*final) = calloc(BUFFER_SIZE, sizeof(char));
        int size = BUFFER_SIZE, len = 0;

        for(int i=0; i<strlen((*init)); i++){
            if(!next[(*init)[i]])
                next[(*init)[i]] = malloc(2), next[(*init)[i]][0] = (*init)[i], next[(*init)[i]][1] = 0;

            const int repl_len = strlen(next[(*init)[i]]);
            while(len + repl_len + 1 >= size){
                char *new_ptr;
                size += BUFFER_SIZE, new_ptr = realloc((*final), size);
                if(!new_ptr){
                    free((*final));
                    free((*init));
                    exit(-1);
                }

                (*final) = new_ptr;
            }
            
            memcpy((*final) + len, next[(*init)[i]], strlen(next[(*init)[i]]));
            len += repl_len;
        }
        free((*init));
        (*init) = (*final);
    }

    // (*final) = strdup((*init));
}
