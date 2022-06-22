// #ifndef THRESHOLDING_C
// #define THRESHOLDING_C

#include "../../header/struct/Image.h"
#include "../../header/struct/matrix.h"
#include "../../header/base_operations/operations.h"
#include "../../header/utilitaire/utilitaire.h"

Matrix thresholding_matrix(Matrix matrix, int threshold)
{
    int i = 0, j = 0;
    int **M = (int **)matrix.M;
    int **M_R = new_int_matrix(matrix.nbr_line, matrix.nbr_colonne);
    for (i = 0; i < matrix.nbr_line; i++)
    {
        for (j = 0; j < matrix.nbr_colonne; j++)
        {
            if (M[i][j] < threshold)
            {
                M_R[i][j] = 1;
            }
        }
    }
    Matrix result = new_matrix(M_R, matrix.nbr_line, matrix.nbr_colonne);
    return result;
}




static double compute_otsu(int *hist, int min, int max, int height, int width)
{
    int i, j;
    double moyen = 0.0, w0G, somme = 0.0, u0, var = 0.0, var1;
    double varience, sq, difference;

    for (i = min; i <= max; i++)
    {
        // wt = wt + hist[i];
        moyen = moyen + i * hist[i];
        somme = somme + hist[i];
    }
    if (somme != 0.0)
    {
        w0G = somme / (height * width); // calcu de la distribution de proba
        u0 = moyen / somme;                // moyenne U1
        for (j = min; j <= max; j++)       // calcul de la varience
        {
            difference = j - u0;
            sq = pow(difference, 2);
            var = var + (sq * hist[j]);
        }
        var1 = var / somme;
        varience = w0G * pow(var1, 2);
        return (varience);
    }
    else
        return (0.0);
}

int otsu(int hauteur, int largeur, Image *img)
{

    double var1, var2, thes, temp, whitin_variance[255], triIntra[255];
    int *hist = histogram(img);

    int i, j, t, threshold;
    // on appelle l'agorithme de otsu pour chaque threshold
    for (i = 0; i < 254; i++)
    {
        var1 = compute_otsu(hist, 0, i, hauteur, largeur);
        var2 = compute_otsu(hist, i + 1, 255, hauteur, largeur);
        whitin_variance[i] = var1 + var2;
        triIntra[i] = whitin_variance[i];
    }

    for (i = 1; i < 254; i++)
    {
        for (j = 1; j < 254 - i; j++)
        {

            if (whitin_variance[i] < whitin_variance[j])
            {
                temp = whitin_variance[i];
                whitin_variance[i] = whitin_variance[j];
                whitin_variance[j] = temp;
            }
        }
    }

    thes = whitin_variance[2];
    for (i = 1; i <= 255; i++)
    {
        if (triIntra[i] == thes)
            threshold = i;
    }

    printf("le threshold min est %d", threshold);
    return (threshold);
}

// #endif