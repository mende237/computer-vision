#ifndef LAPLACIEN_C
#define LAPLACIEN_C
#include "../../../header/filter/high_pass/laplacien.h"
#include "../../convolution/convolution.c"
#include "../../segmentation/thresholding.c"

Image *laplacien(Image *image, int threshold){
    Matrix matrix = new_matrix(image->M, image->nbr_line, image->nbr_col);

    float **L = new_float_matrix(3, 3);
    //ligne1
    L[0][0] = 0;
    L[0][1] = 1;
    L[0][2] = 0;
    //ligne 2
    L[1][0] = 1;
    L[1][1] = -4;
    L[1][2] = 1;
    // ligne 3
    L[2][0] = 0;
    L[2][1] = 1;
    L[2][2] = 0;

    Matrix M_L = new_matrix(L, 3, 3);
    Matrix result = convolution(matrix , M_L , image->val_max);
    char *type = calloc(strlen(image->type), sizeof(char));
    int max = 255;
    char *c = comment("dimitri");
    Image *image_R;
    if (threshold > 0)
    {
        Matrix temp = new_matrix(result.M, image->nbr_line, image->nbr_col);
        strcpy(type, "P1");
        Matrix result = thresholding_matrix(temp, threshold);
        image_R = new_image(result.M, type, c, max, image->nbr_line, image->nbr_col);
        free_matrix(temp, 1);
    }
    else
    {
        strcpy(type, image->type);
        image_R = new_image(result.M, type, c, max, image->nbr_line, image->nbr_col);
    }

    free_matrix(M_L, 1);
    return image_R;
}

#endif