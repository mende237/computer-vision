// #ifndef THRESHOLDING_C
// #define THRESHOLDING_C

#include <math.h>
#include "../../header/struct/Image.h"
#include "../../header/struct/structure.h"
#include "../../header/struct/matrix.h"
#include "../../header/base_operations/operations.h"
#include "../../header/utilitaire/utilitaire.h"

Matrix* thresholding_matrix(Matrix *matrix, int threshold)
{
    int i = 0, j = 0;
    int **M = (int **)matrix->M;
    int **M_R = new_int_matrix(matrix->nbr_line, matrix->nbr_colonne);
    for (i = 0; i < matrix->nbr_line; i++)
    {
        for (j = 0; j < matrix->nbr_colonne; j++)
        {
            if (M[i][j] < threshold)
            {
                M_R[i][j] = 1;
            }
        }
    }
    Matrix *result = new_matrix(M_R, matrix->nbr_line, matrix->nbr_colonne);
    return result;
}

Image *thresholding(Image *image , int automatic , int threshold){
    Matrix *temp = new_matrix(image->M, image->nbr_line, image->nbr_col);
    int S = threshold;
    if (automatic == 1)
    {
        S = otsu(image);
    }
    Matrix *result = thresholding_matrix(temp, S);

    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, "P1");
    Image* image_R = new_image(result->M, type, c, image->val_max, image->nbr_line, image->nbr_col);
    
    // free_matrix(result , 0);
    // free_matrix(temp , 0);
    return image_R;
}

Image *multi_thresholding(Image *image, int *threshold_tab , int *val_tab, int nbr_threshold){
    int **M = (int**) image->M;
    int **M_R = new_int_matrix(image->nbr_line , image->nbr_col); 
    int i = 0, j = 0 , k = 0;
    for (i = 0; i < image->nbr_line; i++)
    {
        for (j = 0; j < image->nbr_col; j++)
        {
            k = 0;
            while (M[i][j] > threshold_tab[k] && k < nbr_threshold)
                k++;

            M_R[i][j] = val_tab[k];
        }
    }

    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, image->type);
    Image *image_R = new_image(M_R , type, c, image->val_max, image->nbr_line, image->nbr_col);
    
    return image_R;
}

static float* compute_otsu(float* hist_norm , int min, int max, int nbr_pixel)
{
    int i = 0, j = 0;
    float *prob_var = calloc(2 , sizeof(float));
    float q = 0 , average = 0 , var = 0;
    
    for (i = min ; i <= max; i++)
    {
        q += hist_norm[i];
    }
    
    if(q != 0){
        prob_var[0] = q;
        for (i = min; i <= max; i++)
        {
            average += i*hist_norm[i];
        }
        average /= q;
        for (i = min; i <= max; i++)
        {
            var += pow(i - average , 2)*hist_norm[i];
        }
        var /= q;
        prob_var[1] = var;
        return prob_var;
    }else{
        return NULL;
    }
}

static int compare(const void *tuple1, const void *tuple2)
{
    typedef struct Tuple Tuple;
    Tuple const *t1 = *((Tuple**) tuple1);
    Tuple const *t2 = *((Tuple**) tuple2);
    float b1 = *((float*) t1->b);
    float b2 = *((float*) t2->b);
    return b1 - b2;
}

int otsu(Image *image)
{
    typedef struct Tuple Tuple;

    double var1 = 0, var2 = 0;
    Tuple **within_variance = calloc(255 , sizeof(Tuple*));
    int i = 0 , j = 0 , nbr_pixel = image->nbr_line*image->nbr_col;

    int *hist = (int*) histogram(image);
    
    float *hist_norm = normalise_histogram(hist , nbr_pixel);

    for (i = 0; i < 255; i++)
    {
        float *prob_var1 = compute_otsu(hist_norm, 0, i, nbr_pixel);
        float *prob_var2 = compute_otsu(hist_norm, i + 1, 255 , nbr_pixel);
        float *var = calloc(1 , sizeof(float));
        if(prob_var1 != NULL && prob_var2 != NULL){
            *var = prob_var1[0] * prob_var1[1] + prob_var2[0] * prob_var2[1];
        }else if(prob_var1 != NULL){
            *var = prob_var1[0] * prob_var1[1];
        }else if(prob_var2 != NULL){
            *var = prob_var2[0] * prob_var2[1];
        }else{
            *var = 0; 
        }

        int *threshold = calloc(1 , sizeof(int));
        *threshold = i;
        Tuple *tuple = new_tuple(threshold , var);
        within_variance[i] = tuple;
        Tuple *t = within_variance[i];
        float b1 = *((float *)t->b);
    }

    qsort(within_variance , 255 , sizeof(Tuple*), compare);
    Tuple *t = within_variance[0];

    int threshold = *((int*) t->a);
    for (i = 0; i < 255; i++)
    {
        free_tuple(within_variance[i] , 1);
    }
    
    free(within_variance);

    
    free(hist);
    free(hist_norm);

    return threshold;
}

// #endif