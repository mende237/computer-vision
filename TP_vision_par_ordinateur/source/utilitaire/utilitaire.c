// #ifndef UTILITAIRE_C
// #define UTILITAIRE_C
//#include "../struct/matrix.c"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../header/utilitaire/utilitaire.h"
#include "../../header/struct/matrix.h"

char *comment(char *c)
{
    char *temp = "# create by ";
    char *result = calloc(strlen(temp) + strlen(c) + 2 , sizeof(char));
    strcat(result , temp);
    strcat(result, c);
    return result;
}

char bit_to_char(int bit)
{
    if (bit == 1)
        return '1';
    else
        return '0';
}

int **new_int_matrix(int nbr_line, int nbr_col)
{
    int i = 0;
    int **M = calloc(nbr_line, sizeof(int *));
    for (i = 0; i < nbr_line; i++)
    {
        M[i] = calloc(nbr_col, sizeof(int));
    }
    return M;
}

float **new_float_matrix(int nbr_line, int nbr_col){
    int i = 0;
    float **M = calloc(nbr_line, sizeof(float *));
    for (i = 0; i < nbr_line; i++)
    {
        M[i] = calloc(nbr_col, sizeof(float));
    }
    return M;
}

int calculate_length_line(FILE *file)
{
    int length = 0;
    char c;
    do
    {
        c = fgetc(file);
        if (c != '\n')
            length++;
    } while (c != '\n');
    // on replace le pointeur dans fichier a la position precedente
    fseek(file, -(length + 1), SEEK_CUR);
    return length;
}

void write_image(char *path, Image *image)
{
    if(image != NULL){

        FILE *file = fopen(path, "w");
        if (file == NULL)
        {
            printf("le fichier %s n'existe pas\n", path);
            exit(EXIT_FAILURE);
        }

        fputs(image->type, file);
        fputc('\n', file);
        fputs(image->comment, file);
        fputc('\n', file);
        fprintf(file, "%d %d\n", image->nbr_col, image->nbr_line);
        // dans le cas ou il ne s'agit pas d'une image binaire on ercit la valeur max
        if (strcasecmp(image->type, "P1") != 0)
            fprintf(file, "%d\n", image->val_max);

        int i = 0, j = 0, cmpt = 0;
        if (strcmp(image->type, "P1") == 0)
        {

            int **M = (int **)image->M;
            for (i = 0; i < image->nbr_line; i++)
            {
                for (j = 0; j < image->nbr_col; j++)
                {
                    if (cmpt == 70)
                    {
                        fputc('\n', file);
                        fputc(bit_to_char(M[i][j]), file);
                        cmpt = 0;
                    }
                    else
                    {
                        fputc(bit_to_char(M[i][j]), file);
                    }
                    cmpt++;
                }
            }
        }
        else if (strcmp(image->type, "P2") == 0)
        {
            int **M = (int **)image->M;
            for (i = 0; i < image->nbr_line; i++)
            {
                for (j = 0; j < image->nbr_col; j++)
                {
                    if (i == image->nbr_line - 1 && j == image->nbr_col - 1)
                    {
                        fprintf(file, "%d", M[i][j]);
                    }
                    else
                    {
                        fprintf(file, "%d\n", M[i][j]);
                    }
                }
            }
        }
        else
        {
            Pixel **M = (Pixel **)image->M;
            for (i = 0; i < image->nbr_line; i++)
            {
                for (j = 0; j < image->nbr_col; j++)
                {
                    if (i == image->nbr_line - 1 && j == image->nbr_col - 1)
                    {
                        fprintf(file, "%d\n%d\n%d", M[i][j].r, M[i][j].g, M[i][j].b);
                    }
                    else
                    {
                        fprintf(file, "%d\n%d\n%d\n", M[i][j].r, M[i][j].g, M[i][j].b);
                    }
                }
            }
        }
        fclose(file);
    }
}

void read_head(FILE *file, Image *image)
{
    int length = 0;
    length = calculate_length_line(file);
    char *comment = calloc(length + 1, sizeof(char));
    fgets(comment, length + 1, file);
    comment[length] = '\0';
    int nbr_line = 0, nbr_col = 0, max_value = 1;
    fgetc(file);
    fscanf(file, "%d %d", &nbr_col, &nbr_line);
    // dans le cas ou il ne s'agit pas d'une image binaire on lit la valeur max
    if (strcasecmp(image->type, "P1") != 0)
        fscanf(file, "%d", &max_value);

    image->comment = comment;
    image->nbr_line = nbr_line;
    image->nbr_col = nbr_col;
    image->val_max = max_value;

    // printf("%s\n", comment);
    // printf("%d  %d\n", nbr_line, nbr_col);
    // printf("%d\n", max_value);
}

void read_PBM(FILE *file, Image *image)
{
    int i = 0, j = 0, temp, cmpt = 0;
    read_head(file, image);
    int **M = new_int_matrix(image->nbr_line, image->nbr_col);
    printf("nbre_line : %d  nbr_col : %d\n", image->nbr_line, image->nbr_col);
    char c;
    do
    {
        c = fgetc(file);
        if (c != '\n' && c != EOF)
        {
            if (c == 49)
                temp = 1;
            else
                temp = 0;

            if (cmpt % image->nbr_col == 0 && cmpt != 0)
                i++;

            M[i][j] = temp;
            if ((j + 1) % image->nbr_col == 0)
                j = 0;
            else
                j++;

            cmpt++;
        }
    } while (c != EOF && cmpt < image->nbr_line * image->nbr_col);
    image->M = M;
}

void read_PGM(FILE *file, Image *image)
{
    int i = 0, j = 0, temp, cmpt = 0;
    read_head(file, image);
    int **M = new_int_matrix(image->nbr_line, image->nbr_col);
    //printf("nbre_line : %d  nbr_col : %d\n", image->nbr_line, image->nbr_col);
    for (cmpt = 0; cmpt < image->nbr_line * image->nbr_col; cmpt++)
    {
        fscanf(file, "%d", &temp);
        if (cmpt % image->nbr_col == 0 && cmpt != 0)
            i++;

        M[i][j] = temp;

        if ((j + 1) % image->nbr_col == 0)
            j = 0;
        else
            j++;
    }
    image->M = M;
}

void read_PPM(FILE *file, Image *image)
{
    int i = 0, j = 0, red = 0, green = 0, blue = 0, cmpt = 0;
    read_head(file, image);
    Pixel **M = calloc(image->nbr_line, sizeof(Pixel *));
    for (i = 0; i < image->nbr_line; i++)
    {
        M[i] = calloc(image->nbr_col, sizeof(Pixel));
    }
    i = 0;
    for (cmpt = 0; cmpt < image->nbr_line * image->nbr_col; cmpt++)
    {
        fscanf(file, "%d\n%d\n%d", &red, &green, &blue);
        if (cmpt % image->nbr_col == 0 && cmpt != 0)
            i++;

        M[i][j].r = red;
        M[i][j].g = green;
        M[i][j].b = blue;

        if ((j + 1) % image->nbr_col == 0)
            j = 0;
        else
            j++;
    }
    image->M = M;
}

Image *read_image(char *path)
{
    Image *image = malloc(sizeof(Image));

    FILE *file = fopen(path, "r");
    if (file == NULL){
        printf("impossible d'ouvrir le fichier %s" , path);
        exit(EXIT_FAILURE);
    }

    int length = 0;
    length = calculate_length_line(file);
    char *type = calloc(length, sizeof(char));
    fscanf(file, "%s", type);
    image->type = type;
    // on ignore le caractere de fin de ligne
    fgetc(file);

    if (strcmp(type, "P1") == 0)
    {
        read_PBM(file, image);
        //printf("binnaire\n");
    }
    else if (strcmp(type, "P2") == 0)
    {
        read_PGM(file, image);
        //printf("niveau de gris\n");
    }
    else if (strcmp(type, "P3") == 0)
    {
        //printf("couleur\n");
        read_PPM(file, image);
    }
    else
    {
        printf("format non pris en charge\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return image;
}

void free_image(Image *image){
    if(image != NULL){
        free(image->comment);
        free(image->type);
        int i = 0, j = 0;
        if(strcmp(image->type , "P1") == 0 || strcmp(image->type , "P2") == 0){
            int **M = image->M;
            for (i = 0; i < image->nbr_line; i++)
            {
                free(M[i]);
            }
            free(image->M);
        }
    }
}

int select_max_matrix(Matrix Mat){
    int i = 0 , j = 0 , max = 0;
    int **M = Mat.M;
    for (i = 0; i < Mat.nbr_line; i++)
    {
        for (j = 0; j < Mat.nbr_colonne; j++)
        {
            if(M[i][j] > max){
                max = M[i][j];
            }
        }
        
    }
    
    return max;
}



struct Point *new_Point(int x, int y, int color)
{
    typedef struct Point Point;
    Point *point = calloc(1, sizeof(Point));
    point->x = x;
    point->y = y;
    point->color = color;
}

boolean equal_point(void *val1, void *val2, ...)
{
    typedef struct Point Point;
    Point *p1 = val1;
    Point *p2 = val2;
    if (p1->x == p2->x && p1->y == p2->y)
        return True;

    return False;
}

void print_image(char *software_name , char *path){
    char *ch = calloc(strlen(software_name) + strlen(path) + 2, sizeof(char));
    strcpy(ch , software_name);
    strcat(ch , " ");
    strcat(ch , path);
    system(ch);
}

struct Tuple *new_tuple(void *a, void *b){
    typedef struct Tuple Tuple;
    Tuple *tuple = calloc(1 , sizeof(Tuple));
    tuple->a = a;
    tuple->b = b;
    return tuple;
}

void free_tuple(struct Tuple *tuple , int free_inner){
    if(free_inner == 1){
        free(tuple->a);
        free(tuple->b);
    }
    free(tuple);
}

Matrix *read_matrix(char *path){
    FILE* file = fopen(path , "r");
    if(file == NULL){
        printf("impossible d'ouvrir le fichier %s" , path);
        exit(EXIT_FAILURE);
    }
    int nbr_line = 0  , nbr_col = 0;
    fscanf(file, "%d %d", &nbr_col, &nbr_line);
    if(nbr_col%2 == 0 || nbr_line%2 == 0){
        printf("la taille de la matrice doit etre impaire");
        exit(EXIT_FAILURE);
    }

    float **M = new_float_matrix(nbr_line , nbr_col);
    int cmpt = 0 , i = 0 , j = 0;
    float temp = 0;
    for (cmpt = 0; cmpt < nbr_line * nbr_col; cmpt++)
    {
        fscanf(file, "%f", &temp);
        if (cmpt % nbr_col == 0 && cmpt != 0)
            i++;

        M[i][j] = temp;

        if ((j + 1) % nbr_col == 0)
            j = 0;
        else
            j++;
    }

    for (i = 0; i < nbr_line; i++)
    {
        for (j = 0; j < nbr_col; j++)
        {
            printf("%f\t",M[i][j]);
        }
        printf("\n");
    }   

    Matrix* matrix = new_matrix(M , nbr_line , nbr_col);
    return matrix;
}

// #endif
