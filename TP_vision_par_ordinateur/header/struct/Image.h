typedef struct Image
{
    char *type;
    char *comment;
    int nbr_line;
    int nbr_col;
    int val_max;
    void *M;
}Image;
