#include "structs.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Font* load_font(char *path_to_file, char **name){
    FILE *file = fopen(path_to_file, "r");
    Font *font_list = NULL;
    if(!file)
        return font_list;

    int cnt = 0, ok = 0;
    while(1){
        char *line;
        read_line(&line, file);
        
        if(!strcmp(line, "ENDFONT")){
            free(line);
            break;
        }

        else if(strnstr(line, "FONT ", 6)){
            (*name) = calloc(strlen(line) + 1, 1);
            strcpy((*name), line + 5);
            ok++;
        }

        else if(strstr(line, "CHARS ")){
            char *end;
            cnt = strtol(line + 6, &end, 10);
            free(line);
            ok++;
            break;
        }

        free(line);
    }

    if(ok != 2)
        return font_list;
    
    font_list = calloc(BUFFER_SIZE, sizeof(Font));

    for(int i=0; i<cnt; i++){
        int enc = 0;
        while(1){
            char *line;
            read_line(&line, file);

            if(strstr(line, "ENDCHAR")){
                free(line);
                break;
            }

            else if(strstr(line, "ENCODING")){
                char *end;
                enc = strtol(line + 9, &end, 10);
            }

            else if(strstr(line, "DWIDTH")){
                char *end;
                font_list[enc].dwx = strtol(line + 7, &end, 10);
                font_list[enc].dwy = strtol(end + 1, &end, 10);
            }

            else if(strstr(line, "BBX")){
                char *end;
                font_list[enc].w = strtol(line + 4, &end, 10);
                font_list[enc].h = strtol(end + 1, &end, 10);
                font_list[enc].x_off = strtol(end + 1, &end, 10);
                font_list[enc].y_off = strtol(end + 1, &end, 10);

                font_list[enc].map = calloc(font_list[enc].h, sizeof(int));
            }

            else if(strstr(line, "BITMAP")){
                for(int i=0; i<font_list[enc].h; i++){
                    free(line);
                    read_line(&line, file);
                    char *end;
                    font_list[enc].map[i] = strtol(line, &end, 16);
                }
            }

            free(line);
        }
    }

    fclose(file);
    return font_list;
}
