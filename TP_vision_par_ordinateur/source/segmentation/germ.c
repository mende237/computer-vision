// #ifndef GERM_C
// #define GERM_C
#include <string.h>
#include "../../header/segmentation/germ.h"
// #include "../data_structure/linked_list.c"
// #include "../data_structure/stack.c"
// #include "../utilitaire/utilitaire.c"

#include "../../header/data_structure/linked_list.h"
#include "../../header/data_structure/stack.h"
#include "../../header/utilitaire/utilitaire.h"

Image *germ(Image *image, int nbr_germ, int threshold)
{
    typedef struct Point Point;
    List* germ_tab = calloc(nbr_germ , sizeof(List));

    int i = 0 , j = 0 , x , y;
    int **M = (int**)image->M;
    for (i = 0; i < nbr_germ; i++)
    {
        x = rand() % image->nbr_line;
        y = rand() % image->nbr_col;
        printf("x %d , y %d\n", x, y);
        Point *p = new_Point(x , y , M[x][y]);
        List germ = spread_germ(p , image , threshold);
        germ_tab[i] = germ;
        printf("taille germ %d\n" , germ->length);
    }
    
    int **M_R = new_int_matrix(image->nbr_line , image->nbr_col);
    for (i = 0; i < nbr_germ; i++)
    {
        List germ = germ_tab[i];
        for (j = 0; j < germ->length; j++)
        {
            Point *p = get_element_list(germ, j);
            M_R[p->x][p->y] = p->color;
        }
    }

    int max = 255;
    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, image->type);
    Image *image_R = new_image(M_R, type, c, max, image->nbr_line, image->nbr_col);

    for (i = 0; i < nbr_germ; i++)
    {
        List germ = germ_tab[i];
        for (j = 0; j < germ->length ; j++)
        {
            Point *p = get_element_list(germ , j);
            free(p);
        }
        free_list(germ);
    }
    
    free(germ_tab);
    return image_R;
}

List good_neighbour(Image *image, struct Point *point, int threshold){
    typedef struct Point Point;
    int i = 0 , j;
    int **M = (int **)image->M;
    List result = new_list();
    for (i = point->x - 1 ; i <= point->x + 1; i++)
    {
        for (j = point->y - 1; j <= point->y + 1; j++)
        {
            if(i < image->nbr_line && i > 0 && j < image->nbr_col && j > 0 && i != point->x && j != point->y){
                if (abs(point->color - M[i][j]) <= threshold)
                {
                    Point *temp_p = new_Point(i , j , M[i][j]);
                    queue_insertion(result , temp_p);
                }
            }
        }
    }

    return result;
}

List spread_germ(struct Point *point , Image *image , int threshold)
{
    int i = 0;
    typedef struct Point Point;
    Stack stack = new_stack();
    List germs = new_list();

    push(stack , point);
    while (is_empty_stack(stack) == False)
    {
        Point *temp_p = pop(stack);
        queue_insertion(germs , temp_p);
        List temp_list = good_neighbour(image , temp_p , threshold);
        for (i = 0; i < temp_list->length; i++)
        {
            Point *temp_p1 = get_element_list(temp_list, i);
            if(search_value_in_list(germs , temp_p1, equal_point) == False){
                push(stack , temp_p1);
            }
        }
        free_list(temp_list);
    }

    free_stack(stack);
    return germs;
}

// #endif