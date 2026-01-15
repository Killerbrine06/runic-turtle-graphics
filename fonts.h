#ifndef FONTS_H
#define FONTS_H

#include "structs.h"
Font* load_font(char *path_to_file, char **name);
void type_text(char *text, int start_x, int start_y, Pixel *color, ProgramState *state);

#endif
