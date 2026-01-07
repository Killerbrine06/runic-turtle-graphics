#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lsystems.h"
#include "utils.h"

LSystem load_lsys(char *path_to_file){
    LSystem sys;
    sys.axiom = malloc(BUFFER_SIZE);
    FILE *lsys_file = fopen(path_to_file, "r");

    if(!lsys_file){
        sys.rules_count = -1;
        return sys;
    }
    read_line(sys.axiom, lsys_file);

    char *line = malloc(BUFFER_SIZE), *end;
    read_line(line, lsys_file);
    printf("%s\n", line);

    sys.rules_count = strtol(line, &end, 10);
    free(line);
    
    sys.rules = malloc(130 * sizeof(char*));

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
