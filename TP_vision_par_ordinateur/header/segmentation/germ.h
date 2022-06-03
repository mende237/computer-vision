#ifndef GERM_H
#define GERM_H
#include "../struct/Image.h"
#include "../data_structure/linked_list.h"
#include "../struct/structure.h"

Image *germ(Image *Image, int nbr_germ, int threshold);
List spread_germ(struct Point *point, Image *image, int threshold);
List good_neighbour(Image *image, struct Point *point, int threshold);
#endif