// #ifndef MATRIX_C
// #define MATRIX_C
#include <stdlib.h>
#include "../../header/struct/matrix.h"

Matrix new_matrix(void *M , int nbr_line, int nbr_colonne)
{
    Matrix Mat;
    Mat.M = M;
    Mat.nbr_line = nbr_line;
    Mat.nbr_colonne = nbr_colonne;
    return Mat;
}

void free_matrix(Matrix M, int free_inner_mat)
{
    if(free_inner_mat != 0){
        int i = 0;
        void **mat = (void**) M.M;
        for (i = 0; i < M.nbr_line; i++)
        {
            free(mat[i]);
        }
        free(mat);
    }
}

// #endif