#ifndef HOUGH_TRANSFORMATION_H
#define HOUGH_TRANSFORMATION_H
#include "../struct/Image.h"
#include "../struct/matrix.h"
#include "../struct/structure.h"

Image* hough_transformation(Image* image);
Image* voting_image(Image *image , float step , char *outline_algorithm);
boolean equal_tuple1(void *tuple1, void*tuple2 ,...);

#endif