#ifndef UTILITAIRE_H
#define UTILITAIRE_H
#include <stdlib.h>
#include <stdio.h>
#include "../struct/pixel.h"
#include "../struct/Image.h"
#include "../struct/matrix.h"
#include "../struct/structure.h"

char *comment(char *c);
Image *read_image(char *path);
void write_image(char *path, Image *image);
int calculate_length_line(FILE *file);
void read_PBM(FILE *file, Image *image);
void read_PGM(FILE *file, Image *image);
void read_PPM(FILE *file, Image *image);
void read_head(FILE *file, Image *image);
int **new_int_matrix(int nbr_line, int nbr_col);
float **new_float_matrix(int nbr_line, int nbr_col);
char bit_to_char(int bit);
int select_max_matrix(Matrix M);
// creation d'un point
struct Point *new_Point(int x, int y, int color);
boolean equal_point(void *val1, void *val2, ...);
struct Tuple *new_tuple(void *a, void *b);
void free_tuple(struct Tuple *tuple , int free_inner);
void free_image(Image *image);
void print_image(char *software_name , char *path);
Matrix* read_matrix(char *path);
#endif