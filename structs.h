#ifndef STRUCTS_H
#define STRUCTS_H
#define TURTLE_STACK_DEF_SIZE 100
typedef struct {
	char *axiom, **rules;
	int rules_count;
} lsystem;

typedef struct {
	unsigned char r, g, b;
} pixel;

typedef struct {
	int w, h;
	pixel **data;
} image;

typedef struct {
	int dwx, dwy, w, h, x_off, y_off;
	int *map;
} font;

typedef struct {
	char *last_output, *font_name;
	int fonts_size;
	image img;
	lsystem lsys;
	font *fonts;
} programstate;

typedef struct stacknode {
	programstate state;
	struct stacknode *next;
} stacknode;

typedef struct turtlestack {
	double x, y, teta;
} turtlestack;

typedef struct turtle {
	unsigned char r, g, b;
	float delta;
	int s_size, s_len, n; // n - numarul de derivari a l-sys
	double x, y, teta, d;
	turtlestack *stack;
} turtle;

#endif