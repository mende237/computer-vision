#ifndef HOUGH_TRANSFORMATION_H
#define HOUGH_TRANSFORMATION_H
#include "../struct/Image.h"
#include "../struct/matrix.h"
#include "../struct/structure.h"


void draw_line(Image *image , float a , float b);
Image *hough_transformation(Image *image_v, Image *image_B, float theta_step, float rho_step, int threshold);
void** voting_image(Image *image, float theta, float rho, char *outline_algorithm);
static float absolute_value(float value);

#endif