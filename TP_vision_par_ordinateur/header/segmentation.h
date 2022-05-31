#ifndef SEGMENTATION_H
#define SEGMENTATION_H
#include "struct/Image.h"
#include "data_structure/linked_list.h"

struct Point
{
    int x;
    int y;
    int color;
};

// creation d'un point
struct Point *new_Point(int x, int y, int color);
// copy d'un point
struct Point *copy_point(const struct Point *Point);
// copy d'un cluster
List copy_cluster(const List cluster);
// copy de la liste des cluster en conservant l'ordre
List *copy_set_of_cluster(const List *cluster_tab, int nbr_cluster);
// calcule du centre d'un cluster
struct Point *calculate_center(List cluster);
// reaffectation des point aux differents cluster
List *reassignment(const List *cluster_tab, const struct Point **center_tab, int nbr_cluster);
// calcule la distance entre deux points
float calculate_distance(const struct Point *point1, const struct Point *point2);
// retourne l'indice et la couleur du centre dont le point passe en parametre est le plus proche
int *nearest_point(const struct Point *point, const struct Point **center_tab, int nbr_cluster);

boolean equal_point(void *val1, void *val2, ...);
// verifie si l'ensemble des clusters est stable
boolean is_stable(const List *new_cluster_tab, const List *old_cluster_tab, int nbr_cluster);
// l'agorithme k_means
Image *k_means(Image *image, int nbr_cluster);

void free_set_of_cluster(List *tab, int nbr_cluster);
void free_cluster(List cluster);

#endif