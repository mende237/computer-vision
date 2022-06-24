#ifndef IMAGE_H
#define IMAGE_H
typedef struct Image
{
    char *type;
    char *comment;
    int nbr_line;
    int nbr_col;
    int val_max;
    void *M;
}Image;

Image *new_image(void *M, char *type, char *comment, int max_value, int nbr_line, int nbr_col);
#endif