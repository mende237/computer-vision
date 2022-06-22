#ifndef THRESHOLDING_H
#define THRESHOLDING_H
#include "../struct/Image.h"
#include "../struct/matrix.h"


static float compute_otsu(int *hist, int min, int max, int height, int width);
int otsu(Image *img);
Matrix thresholding_matrix(Matrix matrix, int threshold);

#endif