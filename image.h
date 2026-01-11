#ifndef IMAGE_H
#define IMAGE_H
#include "structs.h"

Image load_image(char *path_to_file);
void save_image(Image img, char *path_to_file);

#endif
