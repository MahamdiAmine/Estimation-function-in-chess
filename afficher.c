//
// Created by mahamdi on 1/4/19.
//

#include <stdio.h>

/* Affiche la configuration conf */
void affich(struct config conf) {
    int i, j, k;

    for (i = 0; i < 8; i++)
        printf("\t  %c", i + 'a');
    printf("\n");

    for (i = 0; i < 8; i++)
        printf("\t----- ");
    printf("\n");

    for (i = 8; i > 0; i--) {
        printf("    %d", i);
        for (j = 0; j < 8; j++)
            if (conf.mat[i - 1][j] < 0) {
                if (conf.mat[i - 1][j] == -116) printf("\t  ♜");
                if (conf.mat[i - 1][j] == -99) printf("\t  ♞");
                if (conf.mat[i - 1][j] == -102) printf("\t  ♝");
                if (conf.mat[i - 1][j] == -110) printf("\t  ♛");
                if (conf.mat[i - 1][j] == -114) printf("\t  ♚");
                if (conf.mat[i - 1][j] == -112) printf("\t  ♟");
            } else if (conf.mat[i - 1][j] > 0) {
                if (conf.mat[i - 1][j] == 116) printf("\t  ♖");
                if (conf.mat[i - 1][j] == 99) printf("\t  ♘");
                if (conf.mat[i - 1][j] == 102) printf("\t  ♗");
                if (conf.mat[i - 1][j] == 110) printf("\t  ♕");
                if (conf.mat[i - 1][j] == 114) printf("\t  ♔");
                if (conf.mat[i - 1][j] == 112) printf("\t  ♙");
                //printf("\t +%d", conf.mat[i - 1][j]);
            } else printf("\t  ");
        printf("\n");

        for (k = 0; k < 8; k++)
            printf("\t----- ");
        printf("\n");

    }
    printf("\n");

} // affich
/* Affiche la configuration conf */
void affich1(struct config conf) {
    int i, j, k;
    for (i = 0; i < 8; i++)
        printf("\t  %c", i + 'a');
    printf("\n");

    for (i = 0; i < 8; i++)
        printf("\t----- ");
    printf("\n");

    for (i = 8; i > 0; i--) {
        printf("    %d", i);
        for (j = 0; j < 8; j++)
            if (conf.mat[i - 1][j] < 0) printf("\t -%c", -conf.mat[i - 1][j]);
            else if (conf.mat[i - 1][j] > 0) printf("\t +%c", conf.mat[i - 1][j]);
            else printf("\t  ");
        printf("\n");

        for (k = 0; k < 8; k++)
            printf("\t----- ");
        printf("\n");

    }
    printf("\n");

} // affich