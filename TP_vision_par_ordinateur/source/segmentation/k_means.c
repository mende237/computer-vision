// #ifndef K_MEANS_C
// #define K_MEANS_C
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// #include "../data_structure/linked_list.c"
// #include "../../header/segmentation/k_means.h"
// #include "../struct/image.c"
// #include "../utilitaire/utilitaire.c"

#include "../../header/data_structure/linked_list.h"
#include "../../header/segmentation/k_means.h"
#include "../../header/struct/Image.h"
#include "../../header/utilitaire/utilitaire.h"

struct Point *copy_point(const struct Point *point)
{
    return new_Point(point->x, point->y, point->color);
}

List copy_cluster(const List cluster)
{
    List cluster_copy = new_list();
    int i = 0;
    for (i = 0; i < cluster->length; i++)
    {
        queue_insertion(cluster_copy, get_element_list(cluster, i));
    }
    return cluster_copy;
}

List *copy_set_of_cluster(const List *cluster_tab, int nbr_cluster)
{
    int i = 0;
    List *cluster_tab_copy = calloc(nbr_cluster, sizeof(List));
    for (i = 0; i < nbr_cluster; i++)
    {
        cluster_tab_copy[i] = copy_cluster(cluster_tab[i]);
    }

    return cluster_tab_copy;
}

struct Point *calculate_center(const List cluster)
{
    typedef struct Point Point;
    int i = 0, x = 0, y = 0, color;
    Point *p;
    if(is_empty_list(cluster) == True)
        return NULL;
    
    for (i = 0; i < cluster->length; i++)
    {
        p = get_element_list(cluster, i);
        x += p->x;
        y += p->y;
    }
    x /= cluster->length;
    y /= cluster->length;
    color = p->color;
    return new_Point(x, y, color);
}

float calculate_distance(const struct Point *point1, const struct Point *point2)
{
    return sqrt(pow(point2->y - point1->y, 2) + pow(point2->x - point1->x, 2));
}

int* nearest_point(const struct Point *point, const struct Point **center_tab, int nbr_cluster)
{
    int *tuple = calloc(2 ,sizeof(int));
    int i = 0, index = 0 ;
    float d, min = 0.0;
    min = calculate_distance(point, center_tab[0]);
    for (i = 1; i < nbr_cluster; i++)
    {
        if(center_tab[i] != NULL){
            d = calculate_distance(point, center_tab[i]);
            if (d < min)
            {
                min = d;
                index = i;
            }
        }
    }
    tuple[0] = index;
    tuple[1] = center_tab[index]->color;
    return tuple;
}

List *reassignment(const List *cluster_tab, const struct Point **center_tab, int nbr_cluster)
{
    typedef struct Point Point;
    int i = 0, j = 0, index = 0;
    List *new_cluster_tab = calloc(nbr_cluster, sizeof(List));
    for (i = 0; i < nbr_cluster; i++)
    {
        new_cluster_tab[i] = new_list();
    }

    for (i = 0; i < nbr_cluster; i++)
    {
        List cluster = cluster_tab[i];
        if(cluster != NULL){
            for (j = 0; j < cluster->length; j++)
            {
                Point *point = get_element_list(cluster, j);
                int *tuple = nearest_point(point, center_tab, nbr_cluster);
                if (tuple[0] != i)
                {
                    delete_element_list(cluster, j);
                    j--;
                    point->color = tuple[1];
                    queue_insertion(new_cluster_tab[tuple[0]], point);
                }else{
                    point->color = center_tab[i]->color;
                    queue_insertion(new_cluster_tab[i], point);
                }
                free(tuple);
            }
        }
    }

    return new_cluster_tab;
}



boolean is_stable(const List *new_cluster_tab, const List *old_cluster_tab, int nbr_cluster)
{
    boolean stable = True;
    int i = 0, j = 0;
    do
    {
        if (new_cluster_tab[i]->length != old_cluster_tab[i]->length){
            stable = False;
        }else{
            if (include_value_list(new_cluster_tab[i], old_cluster_tab[i], equal_point) == False)
            {
                stable = False;
            }
        }
        i++;
    } while (stable == True && i < nbr_cluster);
    return stable;
}

List *initialise(int **M , int nbr_line , int nbr_col , struct Point **center_tab , int nbr_cluster , List garbage){
    typedef struct Point Point;
    int i = 0 , j = 0;
    List *clusters_tab = calloc(nbr_cluster, sizeof(List));
    List cluster = new_list();
    for (i = 0; i < nbr_line; i++)
    {
        for (j = 0; j < nbr_col; j++)
        {

            Point *point = new_Point(i , j, M[i][j]);
            queue_insertion(garbage, point);
            queue_insertion(cluster , point);
        }
    }

    clusters_tab[0] = cluster; 
    List *result = reassignment(clusters_tab, (const struct Point**)center_tab, nbr_cluster);
    free_set_of_cluster(clusters_tab , nbr_cluster);
    return result;
    
}

Image *k_means(Image *image, int nbr_cluster)
{
    typedef struct Point Point;
    int i = 0, x = 0, y = 0, color , j = 0;
    int **M = (int **)image->M;
    List garbage = new_list();

    // initialisation de la liste des cluster
    
    Point **center_tab = calloc(nbr_cluster , sizeof(Point*));
    for (i = 0; i < nbr_cluster; i++)
    {
        // initialisation des centres des nbr_cluster
        x = rand() % image->nbr_line;
        y = rand() % image->nbr_col;
        color = M[x][y];
        printf("x %d , y %d\n" , x , y);
        Point *point = new_Point(x, y, color);
        queue_insertion(garbage, point);
        center_tab[i] = point;
    }

    //initialisation des cluster
    List *clusters_tab = initialise(M , image->nbr_line , image->nbr_col , center_tab , nbr_cluster , garbage);
    // for (i = 0; i < nbr_cluster; i++)
    // {
    //     int r1 = rand() % clusters_tab[i]->length;
    //     int r2 = rand() % clusters_tab[i]->length;

    //     Point *p1 = get_element_list(clusters_tab[i] , r1);
    //     Point *p2 = get_element_list(clusters_tab[i],  r2);
    //     printf("length :%d color1 : %d color2 : %d\n", clusters_tab[i]->length , p1->color , p2->color);
    // }
    
    //List garbage2 = new_list();
    boolean stable = True;
    do
    {
        center_tab = calloc(nbr_cluster, sizeof(Point *));
        for (i = 0; i < nbr_cluster; i++)
        {
            center_tab[i] = calculate_center(clusters_tab[i]);
        }


        List *new_cluster_tab = reassignment(clusters_tab , (const struct Point**)center_tab, nbr_cluster);
        
        printf("********************old********************\n");
        for (i = 0; i < nbr_cluster; i++)
        {
            int r1 = rand() % clusters_tab[i]->length;
            int r2 = rand() % clusters_tab[i]->length;

            Point *p1 = get_element_list(clusters_tab[i], r1);
            Point *p2 = get_element_list(clusters_tab[i], r2);
            printf("length :%d color1 : %d color2 : %d\n", clusters_tab[i]->length, p1->color, p2->color);
        }


        printf("********************new********************\n");
        for (i = 0; i < nbr_cluster; i++)
        {
            int r1 = rand() % new_cluster_tab[i]->length;
            int r2 = rand() % new_cluster_tab[i]->length;

            Point *p1 = get_element_list(new_cluster_tab[i], r1);
            Point *p2 = get_element_list(new_cluster_tab[i], r2);
            printf("length :%d color1 : %d color2 : %d\n", new_cluster_tab[i]->length, p1->color, p2->color);
        }
        
        stable = is_stable(new_cluster_tab, clusters_tab, nbr_cluster);

        free_set_of_cluster(clusters_tab, nbr_cluster);
        clusters_tab = new_cluster_tab;

        for (i = 0; i < nbr_cluster; i++)
        {
            free(center_tab[i]);
        }
        free(center_tab);
    } while (stable == False);
    //free_list(garbage2);

    int **M_R = new_int_matrix(image->nbr_line , image->nbr_col);
    for (i = 0; i < nbr_cluster; i++)
    {
        List cluster = clusters_tab[i];
        for (j = 0; j < cluster->length; j++)
        {
            Point *p = get_element_list(cluster , j);
            M_R[p->x][p->y] = p->color;
        }
        
    }

    int max = 255;
    char *c = comment("dimitri");
    char *type = calloc(strlen(image->type), sizeof(char));
    strcpy(type, image->type);
    Image *image_R = new_image(M_R , type , c , max , image->nbr_line , image->nbr_col);
    

    for (i = 0; i < garbage->length; i++)
    {
        free(get_element_list(garbage, i));
    }
    free_list(garbage);
    free_set_of_cluster(clusters_tab , nbr_cluster);
    printf("\npasssss!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    return image_R;
}

void free_set_of_cluster(List *tab, int nbr_cluster)
{
    int i = 0;
    for (i = 0; i < nbr_cluster; i++)
    {
        free_cluster(tab[i]);
    }
    free(tab);
}

void free_cluster(List cluster)
{
    free_list(cluster);
}

// #endif