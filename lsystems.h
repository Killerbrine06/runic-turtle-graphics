#ifndef LSYSTEMS_H
#define LSYSTEMS_H

#include "structs.h"
lsystem load_lsys(char *path_to_file);
void deriv(char **init, const int n, char **next, char **final);
#endif
