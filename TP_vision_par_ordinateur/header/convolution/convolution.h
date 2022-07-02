#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include "../struct/matrix.h"
#include "../struct/Image.h"

Matrix* convolution(Matrix *Mat, Matrix *conv_M, int max_value);
Image *convolution_image(Image *image, Matrix *matrix);
#endif