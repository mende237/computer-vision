#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header/convolution/convolution.h"
#include "header/segmentation/k_means.h"
#include "header/segmentation/germ.h"
#include "header/filter/high_pass/gradient.h"
#include "header/filter/high_pass/laplacien.h"
#include "header/filter/low_pass/low_filter.h"
#include "header/transformations/hough_transformation.h"
#include "header/utilitaire/utilitaire.h"
#include "header/base_operations/operations.h"
#include "header/segmentation/thresholding.h"
#include "header/struct/Image.h"
#include "header/data_structure/linked_list.h"


const char *SOFTWARE_NAME = "eog";
const char *HELP_PATH = "./help.txt";


void handle_args(int argc, char *argv[]);
void handle_error_type_image(Image *image, char *operation);
int handle_convert_error(char *nbr, char *operation);
Image *handle_multi_thresholding(Image *image, char *threshold_tab, char *threshold_value, char *path_source);
void load_help();
int is_zero(char *nbr);
List splite(char *ch, char *delim);
void remove_char(char *str, int pos);


void remove_char(char *str, int pos)
{
    int x = 0;
    while (str[x] != '\0')
    {
        if (x >= pos)
        {
            str[x] = str[x + 1];
        }
        x++;
    }
}

List splite(char *ch , char *delim){
    char *str = calloc(strlen(ch) , sizeof(char));
    strcpy(str , ch);
    char *ptr = strtok(str, delim);

    List tokens = new_list();
    while (ptr != NULL)
    {
        queue_insertion(tokens , ptr);
        ptr = strtok(NULL, delim);
    }

    return tokens;
}

Image* handle_multi_thresholding(Image *image , char *threshold_ch, char *value_ch, char *path_source)
{
    //suppression des crochets
    remove_char(threshold_ch, 0);
    remove_char(threshold_ch, strlen(threshold_ch) - 1);

    remove_char(value_ch, 0);
    remove_char(value_ch, strlen(value_ch) - 1);
    List threshold_list = splite(threshold_ch , ",");
    List value_list = splite(value_ch , ",");
    int error = 0;
    if(threshold_list->length != value_list->length - 1){
        load_help();
        error = 1;
        goto END;
    }

    int *threshold_tab = calloc(threshold_list->length , sizeof(int));
    int *value_tab = calloc(value_list->length , sizeof(int));


    int i = 0;
    
    for (i = 0; i < threshold_list->length; i++)
    {
        threshold_tab[i] = handle_convert_error((char*) get_element_list(threshold_list , i) , "SM");
        // printf("%d ", threshold_tab[i]);
    }
    // printf("\n");
    for (i = 0; i < value_list->length; i++)
    {
        value_tab[i] = handle_convert_error((char *)get_element_list(value_list, i) , "SM");
        // printf("%d " , value_tab[i]);
    }

    i = 0;
    while (threshold_tab[i] < threshold_tab[i + 1] && i < threshold_list->length - 1)
        i++;

    if (i != threshold_list->length - 1){
        free(threshold_tab);
        free(value_tab);
        load_help();
        error = 1;
        goto END;
    }

    int nbr_threshold = threshold_list->length;
END:    
    free_list(threshold_list);
    free_list(value_list);
    if(error == 1)
        exit(EXIT_FAILURE);

    Image *image_R = multi_thresholding(image, threshold_tab, value_tab, nbr_threshold);
    free(threshold_tab);
    free(value_tab);
    return image_R;
}

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

void load_help()
{
    FILE *f = fopen(HELP_PATH, "r");
    int car = 0;
    if (f != NULL)
    {
        do
        {
            car = fgetc(f);
            printf("%c", car);
        } while (car != EOF);
        printf("\n");
    }else
        printf("le fichier %s n'existe pas\n" , HELP_PATH);
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
    else if (strcmp(operation, "GE") == 0 || strcmp(operation, "K") == 0)
    {
        if (r > 0)
        {
            return r;
        }
        // chargement de l'aide
        load_help();
        exit(EXIT_FAILURE);
    }
    else if (strcmp(operation, "G") == 0 || strcmp(operation, "S") == 0 || strcmp(operation, "R") == 0 || strcmp(operation, "P") == 0 ||
             strcmp(operation, "L") == 0 || strcmp(operation, "SE") == 0)
    {
        if (strcmp(nbr, "-A") == 0)
        {
            // seuillage automatique
            return -1;
        }
        else if (is_zero(nbr) == 1 || r != 0)
        {
            return r;
        }
        load_help();
        exit(EXIT_FAILURE);
    }
    else if (strcmp(operation, "LT") == 0 || strcmp(operation, "SM") == 0)
    {
        if (is_zero(nbr) == 1 || r != 0)
        {
            if(r < 0 || r > 255){
                load_help();
                exit(EXIT_FAILURE);
            }
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
    }else if(strcmp(image->type , "P2") == 0){
        
    }else{
        if (strcmp(operation, "ADD") == 0 || strcmp(operation, "M") == 0)
        {
            load_help();
            free_image(image);
            exit(EXIT_FAILURE);
        }
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
            image_R = gradient(image, -2);
            write_image(argv[3], image_R);
            printf("enter gradient\n");
        }
        // sobel
        else if (strcmp(argv[1], "-S") == 0)
        {
            image_R = sobel(image, -2);
            write_image(argv[3], image_R);
            printf("enter sobel\n");
        }
        // robert
        else if (strcmp(argv[1], "-R") == 0)
        {
            image_R = robert(image, -2);
            write_image(argv[3], image_R);
            printf("enter sobel\n");
        }
        // prewitt
        else if (strcmp(argv[1], "-P") == 0)
        {
            image_R = prewit(image, -2);
            write_image(argv[3], image_R);
            printf("enter sobel\n");
        }
        // laplacien
        else if (strcmp(argv[1], "-S") == 0)
        {
            image_R = sobel(image, -2);
            write_image(argv[3], image_R);
            printf("enter sobel\n");
        }
        else if (strcmp(argv[1], "-L") == 0)
        {
            image_R = laplacien(image, -2);
            write_image(argv[3], image_R);
            printf("enter laplacien\n");
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
        // l'operateur NOT
        else if (strcmp(argv[1], "-NOT") == 0)
        {
            image_R = not(image);
            write_image(argv[3], image_R);
            printf("inverse image\n");
        }
        else
        {
            load_help();
            exit(EXIT_FAILURE);
        }
        free_image(image);

        if (strcmp(argv[1], "-C") != 0)
        {
            print_image(SOFTWARE_NAME, argv[3]);
            free_image(image_R);
        }
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
            printf("enter gradient seuilage \n");
        }
        // sobel avec seuillage
        else if (strcmp(argv[1], "-S") == 0)
        {
            image = read_image(argv[3]);
            handle_error_type_image(image, "G");
            threshold = handle_convert_error(argv[2], "G");
            printf("enter sobel seuillage \n");
            image_R = sobel(image, threshold);
            write_image(argv[4], image_R);
        }
        // robert avec seuillage
        else if (strcmp(argv[1], "-R") == 0)
        {
            image = read_image(argv[3]);
            handle_error_type_image(image, "R");
            threshold = handle_convert_error(argv[2], "R");
            image_R = robert(image, threshold);
            write_image(argv[4], image_R);
            printf("enter robert\n");
        }
        // prewitt avec seuillage
        else if (strcmp(argv[1], "-P") == 0)
        {
            image = read_image(argv[3]);
            handle_error_type_image(image, "P");
            threshold = handle_convert_error(argv[2], "P");
            image_R = prewit(image, threshold);
            write_image(argv[4], image_R);
            printf("enter prewitt\n");
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
        // seuillage
        else if (strcmp(argv[1], "-SE") == 0)
        {
            printf("enter seuillage\n");
            image = read_image(argv[3]);
            handle_error_type_image(image, "SE");
            threshold = handle_convert_error(argv[2], "SE");
            if (threshold < 0)
            {
                // seuillage automatique
                image_R = thresholding(image, 1, 0);
            }
            else
            {
                image_R = thresholding(image, 0, threshold);
            }
            write_image(argv[4], image_R);
        }
        //addition d'image 
        else if (strcmp(argv[1], "-ADD") == 0)
        {
            printf("addition image \n");
            Image *image1 = read_image(argv[2]);
            handle_error_type_image(image1, "ADD");
            Image *image2 = read_image(argv[3]);
            handle_error_type_image(image2, "ADD");
            image_R = add_PGM_images(image1 , image2);
            write_image(argv[4], image_R);
            free_image(image1);
            free_image(image2);
        }
        //et logique
        else if (strcmp(argv[1], "-AND") == 0)
        {
            printf("et logique image \n");
            Image *image1 = read_image(argv[2]);
            handle_error_type_image(image1, "AND");
            Image *image2 = read_image(argv[3]);
            handle_error_type_image(image2, "AND");
            image_R = and(image1, image2);
            write_image(argv[4], image_R);
            free_image(image1);
            free_image(image2);
        }
        //ou logique
        else if (strcmp(argv[1], "-OR") == 0)
        {
            printf("ou logique image \n");
            Image *image1 = read_image(argv[2]);
            handle_error_type_image(image1, "OR");
            Image *image2 = read_image(argv[3]);
            handle_error_type_image(image2, "OR");
            image_R = or(image1, image2);
            write_image(argv[4], image_R);
            free_image(image1);
            free_image(image2);
        }
        // convolution
        else if (strcmp(argv[1], "-CV") == 0)
        {

        }
        else
        {
            load_help();
            exit(EXIT_FAILURE);
        }

        print_image(SOFTWARE_NAME, argv[4]);
        free_image(image);
        free_image(image_R);
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
        else if (strcmp(argv[1], "-SM") == 0)
        {
            image = read_image(argv[4]);
            handle_error_type_image(image, "SM");
            image_R = handle_multi_thresholding(image , argv[2] , argv[3] , argv[4]);
            write_image(argv[5], image_R);
            printf("seuillage multiple\n");
        } 
        // l'algorithme k-means
        else if (strcmp(argv[1], "-K") == 0)
        {
            image = read_image(argv[4]);
            handle_error_type_image(image, "K");
            int nbr_cluster = handle_convert_error(argv[2], "K");
            int nbr_iter_max = handle_convert_error(argv[3], "K");
            image_R = k_means(image, nbr_cluster , nbr_iter_max);
            write_image(argv[5], image_R);
            printf("k-means\n");
        }
        else
        {
            load_help();
            exit(EXIT_FAILURE);
        }
        print_image(SOFTWARE_NAME, argv[5]);
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
    // int i = 0;
    // char *str = calloc(strlen("strtok needs to be called several times to split a string") ,sizeof(char));
    // strcpy(str, "strtok needs to be called several times to split a string");
    // List list = splite(str, " ");
    // free(str);
    // for (i = 0; i < list->length; i++)
    // {
    //     printf("%s\n" , (char*) get_element_list(list , i));
    //     free(get_element_list(list, i));
    // }
    
    // handle_args(argc, argv);
    // exit(EXIT_SUCCESS);

    // int a = atoi("aqdqsd");
    // printf("%d\n", a);
    // int i = 0, j = 0;
    // Image *image1 = read_image("/home/dimitri/Bureau/marioArretGauche.pgm");
    Image *image_test = read_image("circuit.pgm");
    // Image *image2 = read_image("/home/dimitri/Bureau/marioArretGauche.pgm");
    // Image *image = add_PGM_images(image1, image2);
    Image *image_R;

    //image_R = equal_histogram(image_test);
    // int threshold = otsu(image_test);
    // printf("le seuil est %d\n" , threshold);
    // // Matrix M = new_matrix(image1->M , image1->nbr_line , image1->nbr_col);
    // // int** conv = new_int_matrix(3 , 3);
    // // Matrix conv_M = new_matrix(conv , 3 , 3);

    // /// float c = contraste(image1);
    // /// printf("le contraste est %f\n" , c);
    // // image_R = averaging_filter(image_test, 3, 3);
    // image_R = gaussian_filter(image_test, 3, 3);
    // // image_R = germ(image_test, 7 , 25);
    // // image_R = median_filter(image_test , 3 , 3);
    image_R = voting_image(image_test, 1.0 ,  1.0  , "gradient");

    //image_R = laplacien(image_test, -1);
    // char threshold_ch[] = "[50]";
    // char val[] = "[0,1]";
    // image_R = handle_multi_thresholding(image_test , threshold_ch , val , "oiseau.pgm");
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
    write_image("result/result.pgm", image_R);
    free_image(image_test);
    // free_image(image2);
    // free_image(image);
    free_image(image_R);
    // printf("%d", strlen("1111111111111111111111111111111111111111111111111111111111111111111111"));
    return EXIT_SUCCESS;
}
