#ifndef MATRIX_H
#define MATRIX_H
typedef struct Matrix
{
    void *M;
    int nbr_line;
    int nbr_colonne;
} Matrix;

Matrix *new_matrix(void *M, int nbr_line, int nbr_colonne);
void free_matrix(Matrix *M, int free_inner_mat);
#endif