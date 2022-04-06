#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "source/utilitaire.c"
#include "source/convolution.c"

int main()
{
    Image *image = read_image("/home/dimitri/Bureau/test.pbm");
    write_image("result/result.pbm", image);
    // printf("%d", strlen("1111111111111111111111111111111111111111111111111111111111111111111111"));
    return EXIT_SUCCESS;
}
