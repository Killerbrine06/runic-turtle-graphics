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
    sys.axiom = malloc(BUFFER_SIZE);
    read_line(sys.axiom, lsys_file);

    char *line = malloc(BUFFER_SIZE), *end;
    read_line(line, lsys_file);

    sys.rules_count = strtol(line, &end, 10);
    free(line);

    sys.rules = malloc(ALFABET_SIZE * sizeof(char*));

    for(int i=0; i<sys.rules_count; i++){
        line = malloc(BUFFER_SIZE);
        read_line(line, lsys_file);        
        sys.rules[line[0]] = malloc(strlen(line) - 2);
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
        int size = BUFFER_SIZE;

        for(int i=0; i<strlen((*init)); i++){
            if(!next[(*init)[i]])
                next[(*init)[i]] = malloc(2), next[(*init)[i]][0] = (*init)[i], next[(*init)[i]][1] = 0;

            const int len = strlen((*final));
            if(len + strlen(next[(*init)[i]]) + 1 >= size)
                size += BUFFER_SIZE, *final = realloc((*final), size);
            
            strcat((*final), next[(*init)[i]]);
        }
        free((*init));
        (*init) = strdup((*final));
        free((*final));
    }

    (*final) = strdup((*init));
}
