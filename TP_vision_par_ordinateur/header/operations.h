#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "struct/Image.h"

void *histogram(Image *image);
Image *equal_histogram(Image *image);
Image *linear_transformation(Image *image, int min, int max);
int equal_pixel(void *p1, void *p2, char *image_type);
int pixel_content(int *hist, int val);
Image *add_PGM_images(Image *image1, Image *image2);
Image *multiplication_PGM_images(Image *image1, Image *image2);
float contraste(Image *image);
float contraste_matrix(Matrix M);
#endif