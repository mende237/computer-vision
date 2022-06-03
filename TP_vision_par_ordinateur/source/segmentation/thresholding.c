#ifndef THRESHOLDING_C
#define THRESHOLDING_C
#include "../struct/image.c"
#include "../struct/matrix.c"
#include "../utilitaire/utilitaire.c"

Matrix thresholding_matrix(Matrix matrix, int threshold){
    int i = 0 , j = 0;
    int** M = (int**) matrix.M;
    int** M_R =new_int_matrix(matrix.nbr_line , matrix.nbr_colonne);
    for ( i = 0; i < matrix.nbr_line; i++)
    {
        for (j = 0; j < matrix.nbr_colonne; j++)
        {
            if(M[i][j] < threshold){
                M_R[i][j] = 1;
            }
        }
    }
    Matrix result = new_matrix(M_R , matrix.nbr_line , matrix.nbr_colonne);
    return result;
}




#endif