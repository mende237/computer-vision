#ifndef GRADIEN_H
#define GRADIEN_H
#include "../../struct/Image.h"
#include "../../struct/matrix.h"

Image *robert(Image *image, int threshold);
Matrix* robert_convolution(Matrix* matrix , int **R);

Image *prewit(Image *image, int threshold);

Image *sobel(Image *image, int threshold);

Image *gradient(Image *image , int threshold);
Matrix* gradient_x(Matrix* matrix);
Matrix* gradient_y(Matrix* matrix);

#endif