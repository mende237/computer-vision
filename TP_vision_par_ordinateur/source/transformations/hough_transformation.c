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
#include "../../header/struct/Image.h"

#define PI 3.14159

Image *hough_transformation(Image *image)
{

}

boolean equal_tuple1(void *tuple1, void *tuple2,...)
{
    struct Tuple *t1 = tuple1;
    struct Tuple *t2 = tuple2;
    if (*((float *)t1->a) == *((float *)t2->a))
    {
        return True;
    }

    return False;
}

static float absolute_value(float);
static float absolute_value(float value){
    if(value < 0)
        return -1*value;

    return value;
}

Image *voting_image(Image *image, float theta , float rho, char *outline_algorithm)
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

    float step_theta = PI / (float) nbr_theta;
    float step_rho = pow(image->nbr_line*image->nbr_line + image->nbr_col*image->nbr_col , 0.5) / (float) nbr_rho;

    int **M = image_outline->M;
    int **M_V = new_int_matrix(nbr_theta , nbr_rho);
    for (i = 0; i < image->nbr_line; i++)
    {
        for (j = 0; j < image->nbr_col; j++)
        {
            if(M[i][j] != 1){
                for (t = 0; t < nbr_theta; t++)
                {
                    float temp_theta = t * step_theta;
                    float temp_rho = j*cos(temp_theta) + (image->nbr_line - i)*sin(temp_theta);
                    int temp_int_rho = temp_rho/step_rho;
                    if(temp_int_rho >0 && temp_int_rho < nbr_rho)
                        M_V[t][temp_int_rho] += 1;
                }
            }
        }
    }
    

    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, "P2");
    // Image *image = malloc(sizeof(Image));
    // image->type  = type;
    // image->comment = c;
    // image->val_max = max;
    // image->nbr_line = image1->nbr_line;
    // image->nbr_col = image1->nbr_col;
    // image->M = M;
    Image *image_R = new_image(M_V, type, c, 255, nbr_theta , nbr_rho);
  
    return image_R;
}

// #endif