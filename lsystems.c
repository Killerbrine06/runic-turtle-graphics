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
        *final = (*init);
        return;
    }
    int L = strlen((*init));
    while(n--){
        (*final) = calloc(BUFFER_SIZE, sizeof(char));
        int size = BUFFER_SIZE, len = 0;
        char def_repl[2];

        for(int i=0; i<L; i++){
            char *repl;
            if(!next[(*init)[i]]){
                def_repl[0] = (*init)[i];
                def_repl[1] = '\0';
                repl = def_repl;
            }

            else repl = next[(*init)[i]];
                
            const int repl_len = strlen(repl);
            while(len + repl_len + 1 >= size){
                char *new_ptr;
                size += BUFFER_SIZE, new_ptr = realloc((*final), size);
                if(!new_ptr){
                    free((*final));
                    free((*init));
                    exit(-1);
                }

                (*final) = new_ptr;
                (*final)[len] = 0;
            }
            
            memcpy((*final) + len, repl, repl_len);
            len += repl_len;
        }
        (*final)[len] = 0;
        free((*init));
        (*init) = (*final);
        L = len;
    }

    (*init) = (*final);
    // (*final) = strdup((*init));
}
