// #ifndef CONVOLUTION_C
// #define CONVOLUTION_C

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include "../utilitaire/utilitaire.c"
//#include "../utilitaire/utilitaire.c"
#include "../../header/struct/matrix.h"
#include "../../header/utilitaire/utilitaire.h"
#include "../../header/convolution/convolution.h"


Matrix* convolution(Matrix* Mat, Matrix *conv_Mat, int max_value)
{
    int **M = new_int_matrix(Mat->nbr_line, Mat->nbr_colonne);
    int **matrix = (int **)Mat->M;
    float **conv_matrix = (float **)conv_Mat->M;

    int i = 0, j = 0, k = 0, q = 0, x = 0, y = 0, val = 0;

    int debut_x = conv_Mat->nbr_line / 2;
    int debut_y = conv_Mat->nbr_colonne / 2;
    for (i = debut_x; i < Mat->nbr_line - (conv_Mat->nbr_line / 2); i++)
    {
        for (j = debut_y; j < Mat->nbr_colonne - (conv_Mat->nbr_colonne / 2); j++)
        {
            x = 0;
            y = 0;
            val = 0;
            for (k = i - (conv_Mat->nbr_line / 2); k <= i + (conv_Mat->nbr_line / 2); k++)
            {
                for (q = j - (conv_Mat->nbr_colonne / 2); q <= j + (conv_Mat->nbr_colonne / 2); q++)
                {
                    M[i][j] += matrix[k][q] * conv_matrix[x][y];
                    y++;
                }
                x++;
                y = 0;
            }

            if(M[i][j] < 0)
                M[i][j] = fmax(0 , M[i][j]);
            else if (M[i][j] > max_value)
                M[i][j] = max_value;
        }
    }
    Matrix *result = new_matrix(M, Mat->nbr_line, Mat->nbr_colonne);
    return result;
}

Image* convolution_image(Image* image , Matrix* conv_matrix){
    Matrix *matrix_IN = new_matrix(image->M , image->nbr_line , image->nbr_col);
    Matrix *matrix_R = convolution(matrix_IN , conv_matrix , image->val_max);

    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, image->type);
    Image *image_R = new_image(matrix_R->M , type , c , 255 , image->nbr_line, image->nbr_col);
    
    free_matrix(matrix_R , 0);
    free_matrix(matrix_IN , 0);
    return image_R;
}


// #endif