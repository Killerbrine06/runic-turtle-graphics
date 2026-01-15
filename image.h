#ifndef IMAGE_H
#define IMAGE_H
#include "structs.h"

image load_image(char *path_to_file);
void save_image(image img, char *path_to_file);

#endif
