#ifndef OPERATIONS_C
#define OPERATIONS_C
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../header/operations.h"
#include "struct/image.c"
#include "utilitaire.c"



Image *equal_histogram(Image *image){
    int *hist = (int*)histogram(image);
    float hist_norm[256];
    int i = 0 , j = 0 , nbr_pixel = image->nbr_col*image->nbr_line;
    for (i = 0; i < 256; i++)
    {
        hist_norm[i] = (float) hist[i]/nbr_pixel;
    }

    float C[256];

    for (i = 0; i < 256; i++)
    {
        C[i] = 0.0;
        for (j = 0; j <= i; j++)
        {
            C[i] += hist_norm[j];
        }
        
    }
    int **M = new_int_matrix(image->nbr_line , image->nbr_col);
    int** matrice = (int**) image->M;
    for (i = 0; i < image->nbr_line; i++)
    {
        for (j = 0; j < image->nbr_col; j++)
        {
            M[i][j] = C[matrice[i][j]]*255;
        }
    }
    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, image->type);
    Image *image_R = new_image(M, type, c, image->val_max, image->nbr_line, image->nbr_col);
    return image_R;
}

Image *linear_transformation(Image *image , int min , int max){
    int i = 0 , j = 0;
    int LUT[256];
     
    Image *image_R = malloc(sizeof(Image));
    for (i = 0; i < 256; i++)
    {
        LUT[i] = 255 * (i - min) / (max - min);
    }
    
    if(strcmp(image->type , "P1") == 0){

    }else if(strcmp(image->type , "P2") == 0){
        int** M = new_int_matrix(image->nbr_line , image->nbr_col);
        int **matrice = (int**)image->M;
        for (i = 0; i < image->nbr_line; i++)
        {
            for (j = 0; j < image->nbr_col; j++)
            {
                M[i][j] = (LUT[matrice[i][j]] < 0) ? 0 : LUT[matrice[i][j]];
            }
            
        }
        image_R->M = M;
    }else{


    }

    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, image->type);
    image_R->type = type;
    image_R->comment = c;
    image_R->val_max = image->val_max;
    image_R->nbr_line = image->nbr_line;
    image_R->nbr_col = image->nbr_col;
    //Image *image_R = new_image(M, type, c, max, image1->nbr_line, image1->nbr_col);
    return image_R;
}

void *histogram(Image *image){
    void *hist = NULL;
    int i = 0 , j = 0;
    if(strcmp(image->type , "P1") == 0){
        int **M = (int**)image->M;
        int *temp_hist = (int*)calloc(2 , sizeof(int));
        for (i = 0; i < image->nbr_line; i++)
        {
            for (j = 0; j < image->nbr_col; j++)
            {
                if (pixel_content(temp_hist, M[i][j]) == 1)
                {
                    temp_hist[M[i][j]] += 1;
                }
                else{
                    temp_hist[M[i][j]] = 1;
                }
            }
        }
        hist = temp_hist;
    }
    else if (strcmp(image->type, "P2") == 0)
    {
        int **M = (int**)image->M;
        int *temp_hist = (int *)calloc(256 , sizeof(int));
        for (i = 0; i < image->nbr_line; i++)
        {
            for (j = 0; j < image->nbr_col; j++)
            {
                if (pixel_content(temp_hist, M[i][j]) == 1)
                {
                    temp_hist[M[i][j]] += 1;
                }
                else{
                    temp_hist[M[i][j]] = 1;
                }
            }
        }
        hist = temp_hist;
    }
    else
    {
        
    }
}

float contraste_matrix(Matrix Mat){
    int i = 0, j = 0, S = 0;
    int **M = Mat.M;
    for (i = 0; i < Mat.nbr_line; i++)
    {
        for (j = 0; j < Mat.nbr_colonne; j++)
        {
            S += M[i][j];
        }
    }

    float m = S / (Mat.nbr_line * Mat.nbr_colonne);
    float var = 0;
    for (i = 0; i < Mat.nbr_line; i++)
    {
        for (j = 0; j < Mat.nbr_colonne; j++)
        {
            var += pow((M[i][j] - m), 2);
        }
    }

    var = var / (Mat.nbr_line * Mat.nbr_colonne);
    float ecart_type = sqrt(var);

    return ecart_type;
}

float contraste(Image *image){
    int i = 0 , j = 0 , S = 0;
    int** M = (int**)image->M;
    Matrix Mat;
    Mat.M = M;
    Mat.nbr_line = image->nbr_line;
    Mat.nbr_colonne = image->nbr_col;
    return contraste_matrix(Mat);
}



int pixel_content(int *hist , int val)
{
    int i = 0;
    for (i = 0; i < 256; i++)
    {
        if(i == val)
            return 1;
    }
    return 0;
}

Image* add_PGM_images(Image *image1, Image *image2)
{
    if (image1->nbr_line != image2->nbr_line || image1->nbr_col != image2->nbr_col)
    {
        return NULL;
    }
    else
    {
        int i = 0, j = 0;
        int **M = new_int_matrix(image1->nbr_line, image2->nbr_col);
        int **M1 = (int **)image1->M;
        int **M2 = (int **)image2->M;

        int max = (image1->val_max < image2->val_max) ? image2->val_max : image1->val_max;
        int val = 0;
        for (i = 0; i < image1->nbr_line; i++)
        {
            for (j = 0; j < image1->nbr_col; j++)
            {
                val = M1[i][j] + M2[i][j];
                if (val > max)
                    M[i][j] = max;
                else
                    M[i][j] = val;
            }
        }

        char *c = comment("dimitri");
        char *type = calloc(strlen(image1->type) , sizeof(char));
        strcpy(type , image1->type);
        // Image *image = malloc(sizeof(Image));
        // image->type  = type;
        // image->comment = c;
        // image->val_max = max;
        // image->nbr_line = image1->nbr_line;
        // image->nbr_col = image1->nbr_col;
        // image->M = M;
        Image *image = new_image(M , type , c , max , image1->nbr_line , image1->nbr_col);
        return image;
    }
}

Image* multiplication_PGM_images(Image *image1, Image *image2)
{

}

#endif