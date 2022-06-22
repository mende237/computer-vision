// #ifndef IMAGE_C
// #define IMAGE_C
#include "../../header/struct/Image.h"

Image *new_image(void *M , char *type , char* comment , int max_value , int nbr_line , int nbr_col){
    Image *image = malloc(sizeof(Image));
    image->type = type;
    image->comment = comment;
    image->val_max = max_value;
    image->nbr_line = nbr_line;
    image->nbr_col = nbr_col;
    image->M = M;
    return image;
}

//#endif