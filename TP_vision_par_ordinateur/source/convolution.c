#ifndef CONVOLUTION_C
#define CONVOLUTION_C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utilitaire.c"
#include "operations.c"
#include "../header/convolution.h"
#include "struct/image.c"

Matrix convolution(Matrix Mat, Matrix conv_M)
{
    int **M = new_int_matrix(Mat.nbr_line, Mat.nbr_colonne);
    int **matrix = (int **)Mat.M;
    float **conv_matrix = (float **)conv_M.M;

    
    int i = 0, j = 0, k = 0, q = 0, x = 0, y = 0, val = 0;
   
    int debut_x = conv_M.nbr_line / 2;
    int debut_y = conv_M.nbr_colonne / 2;
    for (i = debut_x; i < Mat.nbr_line - (conv_M.nbr_line / 2); i++)
    {
        for (j = debut_y; j < Mat.nbr_colonne - (conv_M.nbr_colonne / 2); j++)
        {
            x = 0;
            y = 0;
            val = 0;
            for (k = i - (conv_M.nbr_line / 2); k <= i + (conv_M.nbr_line / 2); k++)
            {
                for (q = j - (conv_M.nbr_colonne / 2); q <= j + (conv_M.nbr_colonne / 2); q++)
                {
                    val = val + matrix[k][q] * conv_matrix[x][y];
                    y++;
                }
                x++;
                y = 0;
            }
            M[i][j] = val;
            //printf("%d\n", val);
        }
    }
    Matrix result = new_matrix(M, Mat.nbr_line, Mat.nbr_colonne);
    printf("*******************************************************\n");
    return result;
}

Image *gaussian_filter(Image *image, int height, int width, float amplitude)
{
    Matrix M = new_matrix(image->M, image->nbr_line, image->nbr_col);
    int i = 0, j = 0;
    float **conv = new_float_matrix(height, width);
    float std_x = 0;
    float std_y = 0;

    int center_x = width / 2;
    int center_y = height / 2;
    // ecart type sur x
    {
        int S = 0;
        float m = 0;
        for (i = 0; i < width; i++)
            S += i;

        m = (float)S / width;
        float var = 0;
        for (i = 0; i < width; i++)
            var += pow((i - m), 2);

        var = var / width;

        std_x = sqrt(var);
    }
    // ecart type y
    {
        int S = 0;
        float m = 0;
        for (i = 0; i < height; i++)
            S += i;

        m = (float)S / height;
        float var = 0;

        for (i = 0; i < height; i++)
            var += pow((i - m), 2);

        var = var / height;
        std_y = sqrt(var);
    }
    printf("std_x %f\n", std_x);
    printf("std_y %f\n", std_y);
    float total = 0;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            conv[i][j] = amplitude * exp(-((pow(i - center_x, 2) / (2 * pow(std_x, 2))) + (pow(j - center_y, 2) / (2 * pow(std_y, 2)))));
            total += conv[i][j];
            printf("%f\t", conv[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            conv[i][j] = conv[i][j] / total;

    Matrix conv_M = new_matrix(conv, height, width);
    Matrix M_result = convolution(M, conv_M);
    int max = select_max_matrix(M_result);
    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, image->type);
    Image *image_R = new_image(M_result.M, type, c, max, M_result.nbr_line, M_result.nbr_colonne);
    free_matrix(conv_M, 1);
    return image_R;
}

Image *averaging_filter(Image *image, int height, int width)
{
    Matrix M = new_matrix(image->M, image->nbr_line, image->nbr_col);
    int i = 0, j = 0;
    float **conv = new_float_matrix(height, width);
    int total = height * width;
    for (i = 0; i < height; i++)
    {
        for (j = 0; i < width; i++)
        {
            conv[i][j] = (float)1 / total;
        }
    }

    Matrix conv_M = new_matrix(conv, 3, 3);
    Matrix M_result = convolution(M, conv_M);
    int max = select_max_matrix(M_result);
    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, image->type);
    Image *image_R = new_image(M_result.M, type, c, max, M_result.nbr_line, M_result.nbr_colonne);
}

Image *median_filter(Image *image, int height, int width)
{
    int **M = new_int_matrix(image->nbr_line, image->nbr_col);
    int **matrix = (int **)image->M;
    int i = 0 , j = 0 , k = 0 , q = 0 , cmpt = 0;

    int debut_x = height / 2;
    int debut_y = width / 2;
    int *temp;
    int middle = (height * width)/2;
    for (i = debut_x; i < image->nbr_line - debut_x; i++)
    {
        for (j = debut_y; j < image->nbr_col - debut_y; j++)
        {
            cmpt = 0;
            temp = calloc(height*width , sizeof(int));
            for (k = i - debut_x; k <= i + debut_x; k++)
            {
                for (q = j - debut_y; q <= j + debut_y; q++)
                {
                    temp[cmpt] = matrix[k][q];
                    cmpt++;
                }
            }
            
            M[i][j] = temp[middle - 1];
            if (matrix[i][j] != temp[middle - 1]){
                printf("old new %d %d\n", matrix[i][j], temp[middle-1]);
            }
            free(temp);
        }
    }
    Matrix M_result = new_matrix(M, image->nbr_line, image->nbr_col);

    int max = select_max_matrix(M_result);
    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, image->type);
    Image *image_R = new_image(M_result.M, type, c, max, M_result.nbr_line, M_result.nbr_colonne);
    return image_R;
}

#endif