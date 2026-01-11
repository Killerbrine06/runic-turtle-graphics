#include "image.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Image load_image(char *path_to_file){
    FILE *file = fopen(path_to_file, "rb");

    if(!file){
        printf("Failed to load %s\n", path_to_file);
        Image img;
        img.w = -1;
        return img;
    }

    Image img;
    char magic[3];
    int max_val;
    fscanf(file, "%s", &magic);
    fscanf(file, "%d %d", &img.w, &img.h);
    fscanf(file, "%d", &max_val);
    fgetc(file);

    img.data = malloc(img.h * sizeof(Pixel*));
    for(int i=img.h-1; i>=0; i--){
        img.data[i] = calloc(img.w, sizeof(Pixel));

        for(int j=0; j<img.w; j++){
            fread(&img.data[i][j].r, 1, 1, file);
            fread(&img.data[i][j].g, 1, 1, file);
            fread(&img.data[i][j].b, 1, 1, file);
        }
    }

    fclose(file);
    return img;
}

void save_image(Image img, char *path_to_file){
    FILE *file = fopen(path_to_file, "wb");
    fprintf(file, "P6\n%d %d\n255\n", img.w, img.h);

    for(int i=img.h-1; i>=0; i--)
        for(int j=0; j<img.w; j++){
            fwrite(&img.data[i][j].r, 1, 1, file);
            fwrite(&img.data[i][j].g, 1, 1, file);
            fwrite(&img.data[i][j].b, 1, 1, file);
        }
    
    fclose(file);
}
