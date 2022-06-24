// #ifndef GRADIEN_C
// #define GRADIEN_C
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// #include "../../../header/filter/high_pass/gradient.h"
// #include "../../struct/matrix.c"
// #include "../../utilitaire/utilitaire.c"
// #include "../../convolution/convolution.c"
// #include "../../segmentation/thresholding.c"

#include "../../../header/struct/Image.h"
#include "../../../header/filter/high_pass/gradient.h"
#include "../../../header/struct/matrix.h"
#include "../../../header/utilitaire/utilitaire.h"
#include "../../../header/convolution/convolution.h"
#include "../../../header/segmentation/thresholding.h"

Image *robert(Image *image,  int threshold)
{
    int i = 0 , j = 0;
    Matrix *matrix = new_matrix(image->M , image->nbr_line , image->nbr_col);
    int **M_R = new_int_matrix(image->nbr_line , image->nbr_col);
    // int R_X[2][2] = {{0 , 1},{-1 , 0}};
    // int R_Y[2][2] = {{1, 0}, {0, -1}};

    int **R_X = new_int_matrix(2 , 2);
    R_X[0][0] = 0;
    R_X[0][1] = 1;
    R_X[1][0] = -1;
    R_X[1][1] = 0;

    int **R_Y = new_int_matrix(2 , 2);
    R_Y[0][0] = 1;
    R_Y[0][1] = 0;
    R_Y[1][0] = 0;
    R_Y[1][1] = -1;

    Matrix *M_R_X = robert_convolution(matrix , R_X);
    Matrix *M_R_Y = robert_convolution(matrix , R_Y);

    int **G_R_X = M_R_X->M;
    int **G_R_Y = M_R_Y->M;
    int value = 0;
    for (i = 0; i < image->nbr_line; i++)
    {
        for (j = 0; j < image->nbr_col; j++)
        {
            value = abs(G_R_X[i][j]) + abs(G_R_Y[i][j]);
            M_R[i][j] = (value > image->val_max) ? image->val_max : value;
        }
        
    }

    char *type = calloc(strlen(image->type), sizeof(char));
    int max = 255;
    char *c = comment("dimitri");
    Image *image_R;
    
    if (threshold >= 0)
    {
        Matrix *temp = new_matrix(M_R, image->nbr_line, image->nbr_col);
        strcpy(type, "P1");
        Matrix *result = thresholding_matrix(temp, threshold);
        image_R = new_image(result->M, type, c, max, image->nbr_line, image->nbr_col);
        free_matrix(temp, 1);
    }
    else if(threshold == -1){
        //on ne doit pas liberer cette image !!!!!!!!!!!
        Image *image_temp = new_image(M_R, "P2", "create by dimitri", 255, image->nbr_line, image->nbr_col);
        image_R = thresholding(image_temp, 1, 0);
        for (i = 0; i < image->nbr_line; i++)
            free(M_R[i]);
    }else
    {
        strcpy(type, image->type);
        image_R = new_image(M_R, type, c, max, image->nbr_line, image->nbr_col);
    }

    free_matrix(M_R_X, 1);
    free_matrix(M_R_Y, 1);
    free(R_X);
    free(R_Y);
    return image_R;
}

Matrix *robert_convolution(Matrix *matrix, int **R)
{
    int i = 0 , j = 0 , k = 0 , q = 0;
    int **M = (int **)matrix->M;
    int **M_R = new_int_matrix(matrix->nbr_line , matrix->nbr_colonne);
    int value = 0;
    for (i = 0; i < matrix->nbr_line; i++)
    {
        for (j = 0; j < matrix->nbr_colonne; j++)
        {
            value = 0;
            for (k = 0; k < 2; k++)
            {
                for (q = 0; q < 2 ; q++)
                {
                   if (i + k < matrix->nbr_line && j + q < matrix->nbr_colonne){
                       value += M[i + k][j + q] * R[k][q];
                   }
                }
            }

            M_R[i][j] = (value > 255) ? 255 : value;
        }
    }
    
    Matrix *result = new_matrix(M_R , matrix->nbr_line , matrix->nbr_colonne);   
    return result;
}

Image *prewit(Image *image, int threshold)
{
    int i = 0 , j = 0; 
    Matrix *matrix = new_matrix(image->M , image->nbr_line , image->nbr_col);
    float **P_X = new_float_matrix(3 , 3);
    P_X[0][0] = -1;
    P_X[0][1] = -1;
    P_X[0][2] = -1;
    //ligne 2
    P_X[1][0] = 0;
    P_X[1][1] = 0;
    P_X[1][2] = 0;
    //ligne 3
    P_X[2][0] = 1;
    P_X[2][1] = 1;
    P_X[2][2] = 1;
    float **P_Y = new_float_matrix(3 , 3);
    //colonne 1
    P_Y[0][0] = -1;
    P_Y[1][0] = -1;
    P_Y[2][0] = -1;
    //colonne 2
    P_Y[0][1] = 0;
    P_Y[1][1] = 0;
    P_Y[2][1] = 0;
    //colonne 3
    P_Y[0][2] = 1;
    P_Y[1][2] = 1;
    P_Y[2][2] = 1;

    Matrix *M_P_X = new_matrix(P_X , 3 , 3);
    Matrix *M_P_Y = new_matrix(P_Y , 3 , 3);

    Matrix *G_P_X = convolution(matrix , M_P_X , image->val_max);
    Matrix *G_P_Y = convolution(matrix , M_P_Y , image->val_max);

    int **G_X = G_P_X->M;
    int **G_Y = G_P_Y->M;
    int **M_R = new_int_matrix(image->nbr_line , image->nbr_col);
    int value = 0;
    for (i = 0; i < image->nbr_line; i++)
    {
        for (j = 0; j < image->nbr_col; j++)
        {
            value = G_X[i][j] + G_Y[i][j];
            M_R[i][j] = (value > image->val_max) ? image->val_max : value;
        }
    }

    char *type = calloc(strlen(image->type), sizeof(char));
    int max = 255;
    char *c = comment("dimitri");
    Image *image_R;
    if (threshold >= 0)
    {
        Matrix *temp = new_matrix(M_R, image->nbr_line, image->nbr_col);
        strcpy(type, "P1");
        Matrix *result = thresholding_matrix(temp, threshold);
        image_R = new_image(result->M, type, c, max, image->nbr_line, image->nbr_col);
        free_matrix(temp, 1);
    }
    else if(threshold == -1){
        Image *image_temp = new_image(M_R, "P2", "create by dimitri", 255, image->nbr_line, image->nbr_col);
        image_R = thresholding(image_temp, 1, 0);
        for (i = 0; i < image->nbr_line; i++)
            free(M_R[i]);
    }else
    {
        strcpy(type, image->type);
        image_R = new_image(M_R, type, c, max, image->nbr_line, image->nbr_col);
    }

    free_matrix(M_P_X, 1);
    free_matrix(M_P_Y, 1);

    free_matrix(G_P_X, 1);
    free_matrix(G_P_Y, 1);

    return image_R;
}

Image *sobel(Image *image, int threshold)
{
    int i = 0, j = 0;
    Matrix *matrix = new_matrix(image->M, image->nbr_line, image->nbr_col);
    float **S_X = new_float_matrix(3, 3);
    S_X[0][0] = -1;
    S_X[0][1] = -2;
    S_X[0][2] = -1;
    // ligne 2
    S_X[1][0] = 0;
    S_X[1][1] = 0;
    S_X[1][2] = 0;
    // ligne 3
    S_X[2][0] = 1;
    S_X[2][1] = 2;
    S_X[2][2] = 1;
    float **S_Y = new_float_matrix(3, 3);
    // colonne 1
    S_Y[0][0] = -1;
    S_Y[1][0] = -2;
    S_Y[2][0] = -1;
    // colonne 2
    S_Y[0][1] = 0;
    S_Y[1][1] = 0;
    S_Y[2][1] = 0;
    // colonne 3
    S_Y[0][2] = 1;
    S_Y[1][2] = 2;
    S_Y[2][2] = 1;

    Matrix *M_S_X = new_matrix(S_X, 3, 3);
    Matrix *M_S_Y = new_matrix(S_Y, 3, 3);

    Matrix *G_S_X = convolution(matrix, M_S_X, image->val_max);
    Matrix *G_S_Y = convolution(matrix, M_S_Y, image->val_max);

    int **G_X = G_S_X->M;
    int **G_Y = G_S_Y->M;

    int **M_R = new_int_matrix(image->nbr_line, image->nbr_col);
    int value = 0;
    for (i = 0; i < image->nbr_line; i++)
    {
        for (j = 0; j < image->nbr_col; j++)
        {
            value = G_X[i][j] + G_Y[i][j];
            M_R[i][j] = (value > image->val_max) ? image->val_max : value;
        }
    }

    
    char *type = calloc(strlen(image->type), sizeof(char));
    int max = 255;
    char *c = comment("dimitri");
    Image *image_R;
    if (threshold >= 0)
    {
        Matrix *temp = new_matrix(M_R, image->nbr_line, image->nbr_col);
        strcpy(type, "P1");
        Matrix *result = thresholding_matrix(temp, threshold);
        image_R = new_image(result->M, type, c, max, image->nbr_line, image->nbr_col);
        free_matrix(temp, 1);
    }
    else if(threshold == -1){
        Image *image_temp = new_image(M_R, "P2", "create by dimitri", 255, image->nbr_line, image->nbr_col);
        image_R = thresholding(image_temp , 1 , 0);
        for (i = 0; i < image->nbr_line; i++)
            free(M_R[i]);
    }else
    {
        strcpy(type, image->type);
        image_R = new_image(M_R, type, c, max, image->nbr_line, image->nbr_col);
    }
    free_matrix(M_S_X, 1);
    free_matrix(M_S_Y, 1);

    free_matrix(G_S_X, 1);
    free_matrix(G_S_Y, 1);

    return image_R;
}

Matrix* gradient_x(Matrix* matrix)
{
    int **M_R = new_int_matrix(matrix->nbr_line, matrix->nbr_colonne);
    int **M = (int **)matrix->M;
    int i = 0, j = 0;

    for (i = 0; i < matrix->nbr_line; i++)
    {
        for (j = 0; j < matrix->nbr_colonne - 1; j++)
        {
            M_R[i][j] = (int)fmax(0, M[i][j + 1] - M[i][j]);
        }
    }

    Matrix *result = new_matrix(M_R, matrix->nbr_line, matrix->nbr_colonne);
    return result;
}

Matrix* gradient_y(Matrix* matrix)
{
    int **M_R = new_int_matrix(matrix->nbr_line, matrix->nbr_colonne);
    int **M = (int **)matrix->M;
    int i = 0, j = 0;

    for (i = 0; i < matrix->nbr_line - 1; i++)
    {
        for (j = 0; j < matrix->nbr_colonne; j++)
        {
            M_R[i][j] = (int)fmax(0, M[i][j] - M[i + 1][j]);
        }
    }

    Matrix *result = new_matrix(M_R, matrix->nbr_line, matrix->nbr_colonne);
    return result;
}

Image *gradient(Image *image, int threshold)
{

    int i = 0, j = 0;
    Matrix *matrix = new_matrix(image->M, image->nbr_line, image->nbr_col);
    Matrix *G_X = gradient_x(matrix);
    Matrix *G_Y = gradient_y(matrix);

    int **M_R = new_int_matrix(image->nbr_line, image->nbr_col);
    int **M_X = G_X->M;
    int **M_Y = G_Y->M;
    int value = 0;
    for (i = 0; i < image->nbr_line; i++)
    {
        for (j = 0; j < image->nbr_col; j++)
        {
            value = M_X[i][j] + M_Y[i][j];
            M_R[i][j] = (value > image->val_max) ? image->val_max : value;
        }
    }

    char *type = calloc(strlen(image->type), sizeof(char));
    int max = 255;
    char *c = comment("dimitri");
    Image *image_R;
    if (threshold >= 0)
    {
        Matrix *temp = new_matrix(M_R, image->nbr_line, image->nbr_col);
        strcpy(type, "P1");
        Matrix *result = thresholding_matrix(temp, threshold);
        image_R = new_image(result->M, type , c, max, image->nbr_line, image->nbr_col);
        free_matrix(temp, 1);
    }
    else if(threshold == -1)
    {
        Image *image_temp = new_image(M_R, "P2", "create by dimitri", 255, image->nbr_line, image->nbr_col);
        image_R = thresholding(image_temp, 1, 0);
        for (i = 0; i < image->nbr_line; i++)
            free(M_R[i]);
    }else{
        strcpy(type, image->type);
        image_R = new_image(M_R, type, c, max, image->nbr_line, image->nbr_col);
    }

    free_matrix(G_X, 1);
    free_matrix(G_Y, 1);

    return image_R;
}

// #endif