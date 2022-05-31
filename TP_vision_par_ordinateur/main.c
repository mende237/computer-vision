#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "source/utilitaire.c"
#include "source/convolution.c"
#include "source/operations.c"
#include "source/segmentation.c"
#include "source/struct/matrix.c"

int main()
{
    int i = 0 , j = 0;
    Image *image1 = read_image("/home/dimitri/Bureau/marioArretGauche.pgm");
    Image *image_test = read_image("circuit.pgm");
    Image *image2 = read_image("/home/dimitri/Bureau/marioArretGauche.pgm");
    Image *image = add_PGM_images(image1 , image2);
    Image *image_R;

    //image_R = equal_histogram(image_test);

    // Matrix M = new_matrix(image1->M , image1->nbr_line , image1->nbr_col);
    // int** conv = new_int_matrix(3 , 3);
    // Matrix conv_M = new_matrix(conv , 3 , 3);
    
    ///float c = contraste(image1);
    ///printf("le contraste est %f\n" , c);
    //image_R = averaging_filter(image_test, 3, 3);
    image_R = gaussian_filter(image_test, 3, 3, 100);
    k_means(image1 , 5);

    //image_R = median_filter(image_test, 3, 3);
    //convolution(M , conv_M);
    // printf("\n**************************************************\n%s %s\n" , image->type, image->comment);
    // printf("%d %d\n" , image->nbr_line , image->nbr_col);
    // printf("%d\n",image->val_max);
    // int *tab = histogram(image1);
    // printf("\n");
    // int r = 0;
    // for (i = 0; i < 256; i++)
    // {
    //     r += tab[i];
    //     printf("%d\n", tab[i]);
    // }
    
    // printf("cal = %d , reel = %d\n" , r , image1->nbr_line * image1->nbr_col);
    // free_matrix(M , 0);
    // free_matrix(conv_M , 1);
    write_image("result/result.pgm", image_R);
    free_image(image1);
    free_image(image2);
    free_image(image);
    free_image(image_R);
    // printf("%d", strlen("1111111111111111111111111111111111111111111111111111111111111111111111"));
    return EXIT_SUCCESS;
}
