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

void read_line(char **line, FILE *stream){
    (*line) = calloc(BUFFER_SIZE, 1);
    int size = BUFFER_SIZE, len = 0;
    unsigned char c = 0;
    while(fscanf(stream, "%c", &c) == 1 && c != '\n' && c != EOF) {
        if(len + 2 >= size){
            size += BUFFER_SIZE;
            char *new_ptr = realloc((*line), size);
            if(!new_ptr){
                free((*line));
                exit(-1);
            }

            (*line) = new_ptr;
        }
        (*line)[len++] = c;
    }
}

ProgramState init_state(){
    LSystem l;
    l.axiom = NULL;
    l.rules = NULL;
    l.rules_count = -1;
    Image img;
    img.data = NULL;
    img.w = -1;
    
    ProgramState state;
    state.last_output = strdup("\0");
    state.font_name = NULL;

    state.img = img;
    state.lsys = l;
    state.fonts = NULL;

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
        new_lsys.rules = calloc(ALFABET_SIZE, sizeof(char*));
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
    new_state.last_output = strdup(state.last_output);
    if(state.font_name)
        new_state.font_name = strdup(state.font_name);
    else new_state.font_name = NULL;
    
    new_state.fonts = font_dup(state.fonts);

    return new_state;
}

Font* font_dup(Font *f){
    Font *new = NULL;
    if(!f)
        return new;

    new = calloc(BUFFER_SIZE, sizeof(Font));
    for(int i=0; i<BUFFER_SIZE; i++){
        if(!f[i].map)
            continue;

        new[i].dwx = f[i].dwx;
        new[i].dwy = f[i].dwy;
        new[i].h = f[i].h;
        new[i].w = f[i].w;
        new[i].x_off = f[i].x_off;
        new[i].y_off = f[i].y_off;

        new[i].map = malloc(new[i].h * sizeof(int));
        for(int j=0; j<new[i].h; j++)
            new[i].map[j] = f[i].map[j];
    }

    return new;
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

void free_font(Font *f){
    if(!f)
        return;

    for(int i=0; i<BUFFER_SIZE; i++){
        if(!f[i].map)
            continue;
        
        free(f[i].map);
    }

    free(f);
}

void free_state(ProgramState *state){
    free_lsys(state->lsys);
    free_img(state->img);
    free_font(state->fonts);
    free(state->last_output);

    if(state->font_name)
        free(state->font_name);
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
