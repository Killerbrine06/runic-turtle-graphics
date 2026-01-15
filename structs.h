#ifndef STRUCTS_H
#define STRUCTS_H
#define TURTLE_STACK_DEF_SIZE 100
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
	int dwx, dwy, w, h, x_off, y_off;
	int *map;
} Font;

typedef struct {
	char *last_output, *font_name;
	int fonts_size;
	Image img;
	LSystem lsys;
	Font *fonts;
} ProgramState;

typedef struct StackNode {
	ProgramState state;
	struct StackNode *next;
} StackNode;

typedef struct TurtleStack {
	double x, y, teta;
} TurtleStack;

typedef struct Turtle {
	unsigned char r, g, b;
	float delta;
	int s_size, s_len, n; // n - numarul de derivari a l-sys
	double x, y, teta, d;
	TurtleStack *stack;
} Turtle;

#endif