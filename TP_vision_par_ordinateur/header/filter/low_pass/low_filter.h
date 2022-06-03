#ifndef LOW_FILTER_X
#define LOW_FILTER_X

#include "../../struct/Image.h"

Image *gaussian_filter(Image *image, int height, int width, float amplitude);

Image *averaging_filter(Image *image, int height, int width);

Image *median_filter(Image *image, int height, int width);

#endif

