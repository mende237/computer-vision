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


// #endif