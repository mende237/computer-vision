#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include "struct/matrix.h"

Matrix convolution(Matrix M, Matrix conv_M);
Image *gaussian_filter(Image *image, int height, int width, float amplitude);
Image *averaging_filter(Image *image, int height, int width);
Image *median_filter(Image *image, int height, int width);
#endif