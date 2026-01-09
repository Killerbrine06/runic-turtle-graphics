#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    char *axiom, **rules;
    int rules_count;
} LSystem;

typedef struct {
    unsigned char r, g, b;
} Pixel;

typedef struct {
    int w, h;
    Pixel **data;
} Image;

typedef struct {
    // Image *img;
    LSystem lsys;
    // Font *font;
} ProgramState;

typedef struct StackNode {
    ProgramState state;
    struct StackNode *next;
} StackNode;

#endif