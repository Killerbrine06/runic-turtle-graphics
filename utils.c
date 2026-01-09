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
    l.rules = NULL;
    l.rules_count = -1;
    ProgramState state = {l};
    return state;
}

void clear_stack(StackNode **stack){
    while((*stack))
        pop(stack);
}

void pop(StackNode **stack){
    if(!(*stack))
        return;
    
    if(!(*stack)->next){
        free_lsys((*stack)->state.lsys);
        free((*stack));
        (*stack) = NULL;
        return;
    }

    StackNode *node = (*stack);
    while(node->next->next)
        node = node->next;

    free_lsys(node->next->state.lsys);

    free(node->next);
    node->next = NULL;
}

ProgramState get_head(StackNode *stack){
    StackNode *node = stack;
    while(node->next)
        node = node->next;
    
    return node->state;
}

void push(StackNode **stack, ProgramState state){
    if(!(*stack)){
        StackNode *new_node = malloc(sizeof(StackNode));
        new_node->next = NULL;
        new_node->state = state;
        *stack = new_node;
        return;
    }

    StackNode *node = (*stack);

    while(node->next)
        node = node->next;
    
    StackNode *new_node = malloc(sizeof(StackNode));
    new_node->next = NULL;
    new_node->state = state;
    node->next = new_node;
}

LSystem sys_dup(LSystem lsys){
    LSystem new_lsys;
    new_lsys.rules_count = lsys.rules_count;

    if(lsys.axiom)
        new_lsys.axiom = strdup(lsys.axiom);
    else new_lsys.axiom = NULL;

    if(lsys.rules){
        new_lsys.rules = malloc(ALFABET_SIZE * sizeof(char*));
        for(int i=0; i<ALFABET_SIZE; i++)
            if(lsys.rules[i])
                new_lsys.rules[i] = strdup(lsys.rules[i]);
    } 
    else new_lsys.rules = NULL;

    return new_lsys;
}

ProgramState state_dup(ProgramState state){
    ProgramState new_state;
    new_state.lsys = sys_dup(state.lsys);

    return new_state;
}

void free_lsys(LSystem sys){
    if(sys.axiom)
        free(sys.axiom);
    
    if(sys.rules){
        for(int i=0; i<ALFABET_SIZE; i++)
            if(sys.rules[i])
                free(sys.rules[i]);
    }
}
