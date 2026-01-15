#include "structs.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_header(char **name, FILE *file, int *cnt, int *ok){
	while (1) {
		char *line;
		read_line(&line, file);

		if (!strcmp(line, "ENDFONT")) {
			free(line);
			break;
		}

		else if (_strnstr(line, "FONT ", 6)) {
			(*name) = calloc(strlen(line) + 1, 1);
			strcpy((*name), line + 5);
			(*ok)++;
		}

		else if (strstr(line, "CHARS ")) {
			char *end;
			(*cnt) = strtol(line + 6, &end, 10);
			free(line);
			(*ok)++;
			break;
		}

		free(line);
	}
}

font *load_font(char *path_to_file, char **name, int *list_size)
{
	FILE *file = fopen(path_to_file, "r");
	font *font_list = NULL;
	if (!file)
		return font_list;

	int cnt = 0, ok = 0;
	check_header(name, file, &cnt, &ok);

	if (ok != 2)
		return font_list;

	font_list = calloc(BUFFER_SIZE, sizeof(font));
	int size = BUFFER_SIZE;

	for (int i = 0; i < cnt; i++) {
		int enc = 0;
		while (1) {
			char *line;
			read_line(&line, file);

			if (strstr(line, "ENDCHAR")) {
				free(line);
				break;
			}

			else if (strstr(line, "ENCODING")) {
				char *end;
				enc = strtol(line + 9, &end, 10);

				if (enc < 0)
					enc = 0;

				int n_size = size;
				while (enc >= n_size)
					n_size += BUFFER_SIZE;

				if (n_size != size) {
					font_list = realloc(font_list, n_size * sizeof(font));
					if (!font_list)
						return font_list;

					memset(font_list + size, 0, (n_size - size) * sizeof(font));
					size = n_size;
				}
			}

			else if (strstr(line, "DWIDTH")) {
				char *end, *cursor = line;
				font_list[enc].dwx = strtol(cursor + 7, &end, 10);
				cursor = end;
				font_list[enc].dwy = strtol(cursor + 1, &end, 10);
			}

			else if (strstr(line, "BBX")) {
				char *end, *cursor = line;
				font_list[enc].w = strtol(cursor + 4, &end, 10);
				cursor = end;
				font_list[enc].h = strtol(cursor + 1, &end, 10);
				cursor = end;
				font_list[enc].x_off = strtol(cursor + 1, &end, 10);
				cursor = end;
				font_list[enc].y_off = strtol(cursor + 1, &end, 10);

				if (font_list[enc].map)
					free(font_list[enc].map);

				font_list[enc].map = calloc(font_list[enc].h, sizeof(int));
			}

			else if (strstr(line, "BITMAP")) {
				for (int i = 0; i < font_list[enc].h; i++) {
					free(line);
					read_line(&line, file);
					char *end;
					font_list[enc].map[i] = strtol(line, &end, 16);
				}
			}

			free(line);
		}
	}

	(*list_size) = size;
	fclose(file);
	return font_list;
}

void draw(int x, int y, char **m, int w, int h, pixel *color, image *img)
{
	if (!(0 <= x && x < img->w && 0 <= y && y < img->h))
		return;

	for (int i = h - 1; i >= 0 && y < img->h; i--, y++) {
		int n_x = x;
		for (int j = 0; j < w && n_x < img->w; j++, n_x++)
			if (m[i][j])
				img->data[y][n_x] = (*color);
	}
}

char **char_matrix(font *f)
{
	const int padding = ((f->w + 7) / 8) * 8 - f->w;
	char **m = calloc(f->h, sizeof(char *));
	if (!m)
		return NULL;

	for (int i = 0; i < f->h; i++) {
		m[i] = calloc(f->w, sizeof(char));
		if (!m[i]) {
			for (int j = 0; j < i; j++)
				free(m[j]);

			free(m);
			return NULL;
		}

		int n = f->map[i];
		n >>= padding;

		for (int j = f->w - 1; j >= 0; j--)
			m[i][j] = (n & 1), n >>= 1;
	}

	return m;
}

void type_text(char *text, int start_x, int start_y, pixel *color,
			   programstate *state)
{
	const int L = strlen(text);
	for (int i = 0; i < L; i++) {
		char **m = char_matrix(&state->fonts[(unsigned char)text[i]]);
		if (!m)
			continue;

		draw(start_x + state->fonts[(unsigned char)text[i]].x_off,
			 start_y + state->fonts[(unsigned char)text[i]].y_off, m,
			 state->fonts[(unsigned char)text[i]].w,
			 state->fonts[(unsigned char)text[i]].h, color, &state->img);

		start_x += state->fonts[(unsigned char)text[i]].dwx;
		start_y += state->fonts[(unsigned char)text[i]].dwy;

		for (int j = 0; j < state->fonts[(unsigned char)text[i]].h; j++)
			free(m[j]);
		free(m);
	}
}
