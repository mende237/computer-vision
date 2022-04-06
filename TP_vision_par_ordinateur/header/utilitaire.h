#ifndef UTILITAIRE_H
#define UTILITAIRE_H
#include "struct/pixel.h"
#include "struct/Image.h"
#include <stdlib.h>
#include <stdio.h>

Image* read_image(char *path);
void write_image(char *path , Image *image);
int calculate_length_line(FILE *file);
void read_PBM(FILE *file , Image *image);
void read_PGM(FILE *file , Image *image);
void read_PPM(FILE *file , Image *image);
void read_head(FILE *file , Image *image);
int **new_int_matrice(int nbr_line , int nbr_col);

char bit_to_char(int bit);

#endif