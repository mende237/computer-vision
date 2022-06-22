#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header/convolution/convolution.h"
#include "header/segmentation/k_means.h"
#include "header/segmentation/germ.h"
#include "header/filter/high_pass/gradient.h"
#include "header/filter/high_pass/laplacien.h"
#include "header/transformations/hough_transformation.h"


void handle_args(int argc, char *argv[]);
void handle_error_type_image(Image *image, char *operation);
int handle_convert_error(char *nbr, char *operation);
int is_zero(char *nbr);

int is_zero(char *nbr)
{
    int i = 0;
    for (i = 0; i < strlen(nbr); i++)
    {
        if (nbr[i] != '0')
            return 0;
    }
    return 1;
}
void load_help();

void load_help()
{
    printf("chargement de l'aide\n");
}

int handle_convert_error(char *nbr, char *operation)
{
    int r = atoi(nbr);
    if (strcmp(operation, "FM") == 0 || strcmp(operation, "FMO") == 0 || strcmp(operation, "FG") == 0)
    {
        if (r != 0)
        {
            // la taille des filtre doit etre impaire
            if (r % 2 != 0)
            {
                return r;
            }
            load_help();
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(operation, "GE") == 0 || strcmp(operation, "K") == 0 || strcmp(operation, "G") == 0 || strcmp(operation, "S") == 0 ||
             strcmp(operation, "R") == 0 || strcmp(operation, "P") == 0 || strcmp(operation, "L") == 0)
    {
        if (r > 0)
        {
            return r;
        }
        // chargement de l'aide
        load_help();
        exit(EXIT_FAILURE);
    }
    else if (strcmp(operation, "LT") == 0)
    {
        if (is_zero(nbr) == 1 || r != 0)
        {
            return r;
        }

        load_help();
        exit(EXIT_FAILURE);
    }
    else
    {
        if (r != 0)
        {
            return r;
        }
        load_help();
        exit(EXIT_FAILURE);
    }
    // chargement de l'aide
    load_help();
    exit(EXIT_FAILURE);
}

void handle_error_type_image(Image *image, char *operation)
{
    if (strcmp(image->type, "P3") == 0)
    {
        load_help();
        free_image(image);
        exit(EXIT_FAILURE);
    }
}

void handle_args(int argc, char *argv[])
{
    printf("le nombre d'argument est %d\n", argc);
    if (argc <= 1)
    {
        load_help();
        exit(EXIT_FAILURE);
    }
    else if (argc == 4)
    {
        Image *image;
        Image *image_R;
        // gradient
        image = read_image(argv[2]);
        handle_error_type_image(image, "");
        if (strcmp(argv[1], "-G") == 0)
        {
            image_R = gradient(image, -1);
            write_image(argv[3], image_R);
            printf("enter gradient\n");
        }
        // sobel
        else if (strcmp(argv[1], "-S") == 0)
        {
            image_R = sobel(image, -1);
            write_image(argv[3], image_R);
            printf("enter sobel\n");
        }
        // robert
        else if (strcmp(argv[1], "-R") == 0)
        {
            image_R = robert(image, -1);
            write_image(argv[3], image_R);
            printf("enter sobel\n");
        }
        // prewitt
        else if (strcmp(argv[1], "-P") == 0)
        {
            image_R = prewit(image, -1);
            write_image(argv[3], image_R);
            printf("enter sobel\n");
        }
        // laplacien
        else if (strcmp(argv[1], "-S") == 0)
        {
            image_R = sobel(image, -1);
            write_image(argv[3], image_R);
            printf("enter sobel\n");
        }
        else if (strcmp(argv[1], "-L") == 0)
        {
            image_R = laplacien(image, -1);
            write_image(argv[3], image_R);
            printf("enter sobel\n");
        }
        // egalisation d'histigramme
        else if (strcmp(argv[1], "-EH") == 0)
        {
            image_R = equal_histogram(image);
            write_image(argv[3], image_R);
            printf("equal histogram\n");
        }
        // constraste
        else if (strcmp(argv[1], "-C") == 0)
        {
            float c = contraste(image);
        }
        // transformation lineaire
        else if (strcmp(argv[1], "-LT") == 0)
        {
            image_R = linear_transformation(image, 0, 255);
            write_image(argv[3], image_R);
            printf("linear transformation\n");
        }
        else
        {
            load_help();
            exit(EXIT_FAILURE);
        }
        free_image(image);
        free_image(image_R);
    }
    else if (argc == 5)
    {
        Image *image;
        Image *image_R;
        int threshold;
        if (strcmp(argv[1], "-G") == 0)
        {
            image = read_image(argv[3]);
            handle_error_type_image(image, "G");
            threshold = handle_convert_error(argv[2], "G");
            image_R = gradient(image, threshold);
            write_image(argv[4], image_R);
            printf("enter gradient\n");
        }
        // sobel avec seuillage
        else if (strcmp(argv[1], "-S") == 0)
        {
            image = read_image(argv[3]);
            handle_error_type_image(image, "G");
            threshold = handle_convert_error(argv[2], "G");
            image_R = sobel(image, threshold);
            write_image(argv[4], image_R);
            printf("enter gradient\n");
        }
        // robert avec seuillage
        else if (strcmp(argv[1], "-R") == 0)
        {
            image = read_image(argv[3]);
            handle_error_type_image(image, "G");
            threshold = handle_convert_error(argv[2], "G");
            image_R = robert(image, threshold);
            write_image(argv[4], image_R);
            printf("enter gradient\n");
        }
        // prewitt avec seuillage
        else if (strcmp(argv[1], "-P") == 0)
        {
            image = read_image(argv[3]);
            handle_error_type_image(image, "G");
            threshold = handle_convert_error(argv[2], "G");
            image_R = prewit(image, threshold);
            write_image(argv[4], image_R);
            printf("enter gradient\n");
        }
        // laplacien avec seuillage
        else if (strcmp(argv[1], "-L") == 0)
        {
            image = read_image(argv[3]);
            handle_error_type_image(image, "G");
            threshold = handle_convert_error(argv[2], "G");
            image_R = laplacien(image, threshold);
            write_image(argv[4], image_R);
            printf("enter gradient\n");
        }
        // l'operateur AND
        else if (strcmp(argv[1], "-A") == 0)
        {
        }
        // l'operateur OR
        else if (strcmp(argv[1], "-O") == 0)
        {
        }
        // convolution
        else if (strcmp(argv[1], "-CV") == 0)
        {
        }
        // l'algorithme k-means
        else if (strcmp(argv[1], "-K") == 0)
        {
            image = read_image(argv[3]);
            handle_error_type_image(image, "K");
            int nbr_cluster = handle_convert_error(argv[2], "K");
            image_R = k_means(image, nbr_cluster);
            write_image(argv[4], image_R);
            printf("k-means\n");
        }
        else
        {
            load_help();
            exit(EXIT_FAILURE);
        }
    }
    else if (argc == 6)
    {
        Image *image;
        Image *image_R;
        // transformation lineaire avec saturation
        if (strcmp(argv[1], "-LT") == 0)
        {
            // gestion des erreurs
            int min = handle_convert_error(argv[2], "LT");
            int max = handle_convert_error(argv[3], "LT");
            printf("enter\n");
            if (min >= max)
            {
                load_help();
                exit(EXIT_FAILURE);
            }
            image = read_image(argv[4]);
            handle_error_type_image(image, "LT");
            image_R = linear_transformation(image, min, max);
            write_image(argv[5], image_R);
            printf("linear transformation\n");
        }
        // filtre moyenneur
        else if (strcmp(argv[1], "-FMO") == 0)
        {
            int height = handle_convert_error(argv[2], "FMO");
            int width = handle_convert_error(argv[3], "FMO");
            image = read_image(argv[4]);
            handle_error_type_image(image, "FMO");
            image_R = averaging_filter(image, height, width);
            write_image(argv[5], image_R);
            printf("filtre moyenneur\n");
        }
        // filtre median
        else if (strcmp(argv[1], "-FM") == 0)
        {
            int height = handle_convert_error(argv[2], "FM");
            int width = handle_convert_error(argv[3], "FM");
            image = read_image(argv[4]);
            handle_error_type_image(image, "FM");
            image_R = median_filter(image, height, width);
            write_image(argv[5], image_R);
            printf("filtre median\n");
        }
        // filtre gaussien
        else if (strcmp(argv[1], "-FG") == 0)
        {
            int height = handle_convert_error(argv[2], "FG");
            int width = handle_convert_error(argv[3], "FG");
            image = read_image(argv[4]);
            handle_error_type_image(image, "FG");
            image_R = gaussian_filter(image, height, width);
            write_image(argv[5], image_R);
            printf("filtre moyenneur\n");
        } // algorithme de germ
        else if (strcmp(argv[1], "-GE") == 0)
        {
            int nbr_gem = handle_convert_error(argv[2], "GE");
            int threshold = handle_convert_error(argv[3], "GE");
            image = read_image(argv[4]);
            handle_error_type_image(image, "GE");
            image_R = germ(image, nbr_gem, threshold);
            write_image(argv[5], image_R);
            printf("germ\n");
        }
        else
        {
            load_help();
            exit(EXIT_FAILURE);
        }
        free_image(image);
        free_image(image_R);
    }
    else
    {
        load_help();
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char *argv[])
{
    //handle_args(argc, argv);
    //int *t;
    //compute_otsu(t , 2 , 2 ,3 , 3);
    int a = atoi("aqdqsd");
    printf("%d\n", a);
    // int i = 0, j = 0;
    // Image *image1 = read_image("/home/dimitri/Bureau/marioArretGauche.pgm");
    //Image *image_test = read_image("obscure.pgm");
    // Image *image2 = read_image("/home/dimitri/Bureau/marioArretGauche.pgm");
    // Image *image = add_PGM_images(image1, image2);
    ///Image *image_R;

    // // image_R = equal_histogram(image_test);

    // // Matrix M = new_matrix(image1->M , image1->nbr_line , image1->nbr_col);
    // // int** conv = new_int_matrix(3 , 3);
    // // Matrix conv_M = new_matrix(conv , 3 , 3);

    // /// float c = contraste(image1);
    // /// printf("le contraste est %f\n" , c);
    // // image_R = averaging_filter(image_test, 3, 3);
    // image_R = gaussian_filter(image_test, 3, 3);
    // // image_R = germ(image_test, 7 , 25);
    // // image_R = median_filter(image_test , 3 , 3);
    // //image_R = voting_image(image_test, 0.1, "laplacien");
    //image_R = laplacien(image_test , -1);
    ////image_R = equal_histogram(image_test);
    // //  int a = 10 , b = 15;
    // //  struct Tuple *tuple = new_tuple(&a , &b);
    // //  int *c = tuple->a;
    // //  int *d = tuple->b;

    // // printf("%d  %d", *((int *)tuple->a), *((int *)tuple->b));
    // // free_tuple(tuple , 1);

    // // float pi = 3.14;
    // // int length = pi/0.2;
    // // printf("%d" , length);
    // // image_R = median_filter(image_test, 11, 11);
    // // convolution(M , conv_M);
    // //  printf("\n**************************************************\n%s %s\n" , image->type, image->comment);
    // //  printf("%d %d\n" , image->nbr_line , image->nbr_col);
    // //  printf("%d\n",image->val_max);
    // //  int *tab = histogram(image1);
    // //  printf("\n");
    // //  int r = 0;
    // //  for (i = 0; i < 256; i++)
    // //  {
    // //      r += tab[i];
    // //      printf("%d\n", tab[i]);
    // //  }

    // // printf("cal = %d , reel = %d\n" , r , image1->nbr_line * image1->nbr_col);
    // // free_matrix(M , 0);
    // // free_matrix(conv_M , 1);
   // write_image("result/result.pgm", image_R);
    //free_image(image_test);
    // free_image(image2);
    // free_image(image);
    //free_image(image_R);
    // printf("%d", strlen("1111111111111111111111111111111111111111111111111111111111111111111111"));
    return EXIT_SUCCESS;
}
