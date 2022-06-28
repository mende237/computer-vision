// #ifndef HOUGH_TRANSFORMATION_C
// #define HOUGH_TRANSFORMATION_C
#include <string.h>
#include <math.h>
#include <search.h>
#include <stdlib.h>

#include <stdio.h>
#include "../../header/transformations/hough_transformation.h"
#include "../../header/utilitaire/utilitaire.h"
#include "../../header/struct/structure.h"
#include "../../header/filter/high_pass/gradient.h"
#include "../../header/filter/high_pass/laplacien.h"
#include "../../header/data_structure/linked_list.h"
#include "../../header/segmentation/thresholding.h"
#include "../../header/data_structure/linked_list.h"
#include "../../header/struct/Image.h"

#define PI 3.14159

void draw_line(Image *image, float a, float b){
    int x = 0 , i = 0 , j = 0;
    int **M = image->M;
    // for (i = 0; i < image->nbr_line; i++)
    // {
    //     for (j = 0; j < image->nbr_col; j++)
    //     {
    //         M[i][j] = 1;
    //     }
        
    // }
    

    for (x = 0; x < image->nbr_col; x++)
    {
        int y = (image->nbr_line - a*x)/b;
        if(y >= 0 && y < image->nbr_line)
            M[y][x] =  0;
    }
}

Image *hough_transformation(Image *image_v, Image *image_B , float theta_step , float rho_step, int threshold)
{
    typedef struct Tuple Tuple;
    int **M_V = (int **) image_v->M;
    int i = 0, j = 0;
    List list = new_list();

    for (i = 0; i < image_v->nbr_line; i++)
    {
        for (j = 0; j < image_v->nbr_col ; j++)
        {
            if(M_V[i][j] >= threshold){
                float *theta = calloc(1 , sizeof(float));
                float *rho = calloc(1 , sizeof(float));
                *theta = theta_step * i;
                *rho = rho_step * j; 
                Tuple *tuple = new_tuple(theta , rho);
                queue_insertion(list , tuple);
            }
        }
    }

    
    for (i = 0; i < list->length; i++)
    {
        Tuple *tuple = get_element_list(list , i);
        float a = *((float *)tuple->b) * cos(*((float*) tuple->a));
        float b = *((float *)tuple->b) * sin(*((float *)tuple->a));

        if(a >= 1){
            printf("%f %f\n" , a , b);
        }

        draw_line(image_B , a , b);
    }

    for (i = 0; i < list->length; i++)
    {
        free_tuple(get_element_list(list , i) , 1);
    }
    free_list(list);
}


static float absolute_value(float value){
    if(value < 0)
        return -1*value;

    return value;
}

void** voting_image(Image *image, float theta , float rho, char *outline_algorithm)
{
    Image *image_outline = NULL;
    if (strcmp(outline_algorithm, "sobel") == 0)
    {
        image_outline = sobel(image, -1);
    }
    else if (strcmp(outline_algorithm, "robert") == 0)
    {
        image_outline = robert(image, -1);
    }
    else if (strcmp(outline_algorithm, "prewit") == 0)
    {
        image_outline = prewit(image, -1);
    }
    else if (strcmp(outline_algorithm, "laplacien") == 0)
    {
        image_outline = laplacien(image, -1);
    }
    else
    {
        image_outline = gradient(image, -1);
    }

    int i = 0 , j = 0 , t = 0;
    int nbr_theta = 180 / theta;
    int nbr_rho = pow(image->nbr_line * image->nbr_line + image->nbr_col * image->nbr_col , 0.5)/ (float)rho;

    float *step_theta = calloc(1 , sizeof(float));
    *step_theta = PI / (float) nbr_theta;
    float *step_rho = calloc(1 , sizeof(float));
    *step_rho = pow(image->nbr_line*image->nbr_line + image->nbr_col*image->nbr_col , 0.5) / (float) nbr_rho;

    int **M = image_outline->M;
    int **M_V = new_int_matrix(nbr_theta , nbr_rho);
    for (i = 0; i < image->nbr_line; i++)
    {
        for (j = 0; j < image->nbr_col; j++)
        {
            if(M[i][j] != 1){
                for (t = 0; t < nbr_theta; t++)
                {
                    float temp_theta = t * (*step_theta);
                    float temp_rho = j*cos(temp_theta) + (image->nbr_line - i)*sin(temp_theta);
                    int temp_int_rho = temp_rho/(*step_rho);
                    if(temp_int_rho >0 && temp_int_rho < nbr_rho)
                        M_V[t][temp_int_rho] += 1;
                }
            }
        }
    }
    

    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, "P2");
    Image *image_R = new_image(M_V, type, c, 255, nbr_theta , nbr_rho);
  
    void** tab = calloc(4 , sizeof(void*));
    tab[0] = image_R ;
    tab[1] = image_outline;
    tab[2] = step_theta;
    tab[3] = step_rho;
    return tab;
}

// #endif