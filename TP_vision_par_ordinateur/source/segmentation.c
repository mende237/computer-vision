#ifndef SEGMENTATION_C
#define SEGMENTATION_C
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "data_structure/linked_list.c"
#include "../header/segmentation.h"
#include "struct/image.c"
#include "utilitaire.c"

struct Point *new_Point(int x, int y, int color)
{
    typedef struct Point Point;
    Point *point = calloc(1, sizeof(Point));
    point->x = x;
    point->y = y;
    point->color = color;
}

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
        for (j = 0; j < cluster->length; j++)
        {
            Point *point = get_element_list(cluster, j);
            int *tuple = nearest_point(point, center_tab, nbr_cluster);
            if (tuple[0] != i)
            {
                delete_element_list(cluster, j);
                j--;
                point->color = tuple[1];
                queue_insertion(new_cluster_tab[i], point);
            }
            free(tuple);
        }
    }

    return new_cluster_tab;
}

boolean equal_point(void *val1, void *val2, ...)
{
    typedef struct Point Point;
    Point *p1 = val1;
    Point *p2 = val2;
    if (p1 == p2)
        return True;

    return False;
}

boolean is_stable(const List *new_cluster_tab, const List *old_cluster_tab, int nbr_cluster)
{
    boolean stable = True;
    int i = 0, j = 0;
    do
    {
        if (include_value_list(new_cluster_tab[i], old_cluster_tab[i], equal_point) == False)
        {
            stable = False;
        }
        i++;
    } while (stable == True && i < nbr_cluster);
    return stable;
}

Image *k_means(Image *image, int nbr_cluster)
{
    printf("\nenter!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    typedef struct Point Point;
    int i = 0, x = 0, y = 0, color;
    int **M = (int **)image->M;
    List garbage = new_list();

    // initialisation de la liste des cluster
    List *clusters_tab = calloc(nbr_cluster, sizeof(List));

    for (i = 0; i < nbr_cluster; i++)
    {
        // initialisation des centres des nbr_cluster
        x = rand() % image->nbr_line;
        y = rand() % image->nbr_col;
        color = M[x][y];

        Point *point = new_Point(x, y, color);
        queue_insertion(garbage, point);
        List cluster = new_list();
        queue_insertion(cluster, point);
        clusters_tab[i] = cluster;
    }
    //List garbage2 = new_list();
    boolean stable = True;
    do
    {
        Point **center_tab = calloc(nbr_cluster, sizeof(Point *));
        for (i = 0; i < nbr_cluster; i++)
        {
            center_tab[i] = calculate_center(clusters_tab[i]);
        }

        List *new_cluster_tab = reassignment(clusters_tab , center_tab, nbr_cluster);
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

    for (i = 0; i < garbage->length; i++)
    {
        free(get_element_list(garbage, i));
    }
    free_list(garbage);
    printf("\npasssss!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    return NULL;
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
    // int i = 0;
    // for (i = 0; i < cluster->length; i++)
    // {
    //     free(get_element_list(cluster , i));
    // }
    free_list(cluster);
}

#endif