#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "../struct/Image.h"
#include "../struct/matrix.h"

void *histogram(Image *image);
Image *equal_histogram(Image *image);
float* normalise_histogram(int *hist , int nbr_pixel);
Image *linear_transformation(Image *image, int min, int max);
int equal_pixel(void *p1, void *p2, char *image_type);
int pixel_content(int *hist, int val);
Image *add_PGM_images(Image *image1, Image *image2);
Image *multiplication_PGM_images(Image *image1, float coef);
Image *and (const Image *image1, const Image *image2);
Image *or (const Image *image1, const Image *image2);
Image *not(const Image *image);
float contraste(Image * image);
float contraste_matrix(Matrix M);
#endif