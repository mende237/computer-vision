#ifndef LOW_FILTER_H
#define LOW_FILTER_H

#include "../../struct/Image.h"

Image *gaussian_filter(Image *image, int height, int width);

Image *averaging_filter(Image *image, int height, int width);

Image *median_filter(Image *image, int height, int width);

#endif

