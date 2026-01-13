#include "turtle.h"
#include "structs.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

void append_to_stack(Turtle *t){
    if(t->s_len + 1 >= t->s_size)
        t->s_size += TURTLE_STACK_DEF_SIZE, t->stack = realloc(t->stack, t->s_size * sizeof(TurtleStack));

    t->stack[t->s_len].x = t->x;
    t->stack[t->s_len].y = t->y;
    t->stack[t->s_len++].teta = t->teta;
}

void pop_stack(Turtle *t){
    t->x = t->stack[t->s_len-1].x;
    t->y = t->stack[t->s_len-1].y;
    t->teta = t->stack[t->s_len-1].teta;
    t->s_len--;
}

void draw_line(Image *img, int x0, int y0, int x1, int y1, Pixel p){
    int dx = abs(x1 - x0);
    int sx = (x0 < x1 ? 1 : -1);

    int dy = -abs(y1 - y0);
    int sy = (y0 < y1 ? 1 : -1);

    int err = dx + dy;
    while(1){
        if(0 <= x0 && x0 < img->w && 0 <= y0 && y0 < img->h)
            img->data[y0][x0] = p;

        if(x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if(e2 >= dy)
            err += dy, x0 += sx;
        
        if(e2 <= dx)
            err += dx, y0 += sy;
    }
}

void move(Turtle *t, Image *img){
    double x0 = t->x, y0 = t->y;
    double x1 = x0 + t->d * cos(t->teta * M_PI / 180);
    double y1 = y0 + t->d * sin(t->teta * M_PI / 180);

    // if(x1 >= img->w)
    //     x1 = (double)(img->w - 1);
    
    // else if (x1 < 0)
    //     x1 = (double)0;
    
    // if(y1 >= img->h)
    //     y1 = (double)(img->h - 1);
    
    // else if(y1 < 0)
        // y1 = (double)0;
    
    const Pixel p = {t->r, t->g, t->b};

    draw_line(img, (int)lround(x0), (int)lround(y0), (int)lround(x1), (int)lround(y1), p);
    t->x = x1;
    t->y = y1;
}

Image execute_string(Turtle *t, char *s, const Image img){
    Image new_img = img_dup(img);
    t->s_len = 0;
    t->s_size = TURTLE_STACK_DEF_SIZE;
    t->stack = malloc(t->s_size * sizeof(TurtleStack));

    for(int i=0; i<strlen(s); i++){
        switch(s[i]){
            case '+':
                t->teta += t->delta;
                break;
            
            case '-':
                t->teta -= t->delta;
                break;
            
            case '[':
                append_to_stack(t);
                break;
            
            case ']':
                pop_stack(t);
                break;
            
            case 'F':
                move(t, &new_img);
                break;

            default:
                break;
        }
    }

    return new_img;
}
