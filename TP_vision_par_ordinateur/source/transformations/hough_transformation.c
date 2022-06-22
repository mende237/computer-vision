#ifndef HOUGH_TRANSFORMATION_C
#define HOUGH_TRANSFORMATION_C
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

Image *voting_image(Image *image, float step, char *outline_algorithm)
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

    int length = PI / step;
    printf("--------------------%d , %f  %f\n" ,length, step , PI);
    int i = 0, j = 0, cmpt = 0;
    float theta = 0.0;
    typedef struct Tuple Tuple;
    
   

    char** key_tab = calloc(image->nbr_line * image->nbr_col , sizeof(char*));

    hcreate(image->nbr_line * image->nbr_col);
    ENTRY e;
    ENTRY *dic_element;

    int **M = image_outline->M;
    int q = 0;
    for (i = 0; i < image->nbr_line; i++)
    {
        printf("%d\n" , i);
        for (j = 0; j < image->nbr_col; j++)
        {
            if(M[i][j] != 0){
                cmpt = 0;
                for (theta = 0; theta <= PI; theta += step)
                {
                    float rho = i*cos(theta) + j*sin(theta);
                    char *key = calloc(20 , sizeof(char));
                    sprintf(key , "%f" , rho);
                    key_tab[q] = key;
                    e.key = key;
                    dic_element = hsearch(e, FIND);
                    int *temp;
                    if (dic_element == NULL)
                    {
                        temp = calloc(length + 1, sizeof(int));
                        temp[cmpt]++;
                        e.data = temp;
                        dic_element = hsearch(e, ENTER);
                    }
                    else
                    {
                        temp = dic_element->data;
                        temp[cmpt]++;
                    }
                }
            }else{
                //printf("enter");
            }
            q++;
        }
    }

    int **M_R = new_int_matrix(image->nbr_line + image->nbr_col , image->nbr_line + image->nbr_col);
    for (i = 0; i < image->nbr_line * image->nbr_col; i++)
    {
        if (key_tab[i] != NULL){
            e.key = key_tab[i];
            dic_element = hsearch(e, FIND);
            int *temp = e.data;
            for (j = 0; j < length; j++)
            {
                int x =  abs(atof(key_tab[i])*cos(temp[j]));
                int y =  abs(atof(key_tab[i])*sin(temp[j]));
                M_R[x][y] = 20;
            }
        }
    }

    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, image_outline->type);
    // Image *image = malloc(sizeof(Image));
    // image->type  = type;
    // image->comment = c;
    // image->val_max = max;
    // image->nbr_line = image1->nbr_line;
    // image->nbr_col = image1->nbr_col;
    // image->M = M;
    Image *image_R = new_image(M_R, type, c, 255, image_outline->nbr_line * image_outline->nbr_col, image_outline->nbr_line * image_outline->nbr_col);
    for (i = 0; i < image_outline->nbr_line * image_outline->nbr_col; i++)
    {
        free(key_tab[i]);
    }
    
    free(key_tab);
    hdestroy();
    return image_R;
}

#endif