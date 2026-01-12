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

    fgets(line, BUFFER_SIZE, stream);
    if(line[strlen(line) - 1] == '\n')
        line[strlen(line) - 1] = 0;
}

ProgramState init_state(){
    LSystem l;
    l.axiom = NULL;
    l.rules = NULL;
    l.rules_count = -1;
    Image img;
    img.data = NULL;
    img.w = -1;
    ProgramState state = {img, l};
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
        free_state((&(*stack)->state));
        free((*stack));
        (*stack) = NULL;
        return;
    }

    StackNode *node = (*stack);
    while(node->next->next)
        node = node->next;

    free_state((&node->next->state));

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
    new_state.img = img_dup(state.img);

    return new_state;
}

Image img_dup(Image img){
    Image new_img;
    new_img.w = img.w;
    new_img.h = img.h;

    if(!img.data){
        new_img.data = NULL;
        return new_img;
    }

    new_img.data = malloc(img.h * sizeof(Pixel*));
    for(int i=0; i<img.h; i++){
        if(img.data[i]){
            new_img.data[i] = malloc(img.w * sizeof(Pixel));
            memcpy(new_img.data[i], img.data[i], img.w * sizeof(Pixel));
        }
    }

    return new_img;
}

void free_lsys(LSystem sys){
    if(sys.axiom)
        free(sys.axiom);
    
    if(sys.rules){
        for(int i=0; i<ALFABET_SIZE; i++)
            if(sys.rules[i])
                free(sys.rules[i]);
        
        free(sys.rules);
    }
}

void free_img(Image img){
    if(!img.data)
        return;
    
    for(int i=0; i<img.h; i++)
        if(img.data[i])
            free(img.data[i]);
    
    free(img.data);
}

void free_state(ProgramState *state){
    free_lsys(state->lsys);
    free_img(state->img);
}

void update_state(StackNode **undo_stack, StackNode **redo_stack, ProgramState *current_state, ProgramState *new_state){
    clear_stack(redo_stack);
    push(undo_stack, state_dup((*current_state)));
    
    free_state(current_state);
    (*current_state) = state_dup((*new_state));
    free_state(new_state);
}

void get_turtle_args(Turtle *t, char *cmd){
    char *cuv = strtok(cmd, " "), *end;
    t->x = strtold(cuv, &end);
    cuv = strtok(NULL, " ");
    t->y = strtold(cuv, &end);
    cuv = strtok(NULL, " ");
    t->d = strtold(cuv, &end);
    
    cuv = strtok(NULL, " ");
    t->teta = strtold(cuv, &end);
    cuv = strtok(NULL, " ");
    t->delta = strtod(cuv, &end);

    cuv = strtok(NULL, " ");
    t->n = strtol(cuv, &end, 10);

    cuv = strtok(NULL, " ");
    t->r = strtol(cuv, &end, 10);
    cuv = strtok(NULL, " ");
    t->g = strtol(cuv, &end, 10);
    cuv = strtok(NULL, " ");
    t->b = strtol(cuv, &end, 10);
}
