//
// Created by mahamdi on 12/30/18.
//


#include <stdbool.h>
#include <stdio.h>

// Type d'une configuration formant l'espace de recherche
struct config {
    char mat[8][8];            // l'échiquier
    int val;            // estimation de la config
    char xrN, yrN, xrB, yrB;    // positions des rois Noir et Blanc
    char roqueN, roqueB;        // indicateurs de roque pour N et B :
    // 'g'rand roque non réalisable
    // 'p'etit roque non réalisable
    // 'n'on réalisable des 2 cotés
    // 'r'éalisable (valeur initiale)
    // 'e'ffectué
};
int Cadence = 2;// it's a Rapid game
/*Cadence ==1 Clasical game
Cadence ==2 Rapid game
Cadence ==3 Blitz game
*/

int Pion_isole = -30;
int Pion_double = -30;
int Tour_colonne_ouverte = 70;
int Tour_colonne_semi_ouverte = 50;
int Dame_colonne_ouverte = 30;
int Dame_colonne_semi_ouverte = 22;
int Paire_de_fous = 50;
int Securite_du_roi = 40;
int Countre_attack = 30;
int Echec_perpetual = 40;
int Pion_passe = 400;     //pion passé
int Pion_passe_connecte = 910;     //pion passé
int Sacrifice_positionnelle = 20;
int Sacrifice = -40;
int Coups_intuitifs_countre_coups_non_trivials = 60;
int Avantage_pendule = 0;
int Roque = 0;


int PawnTable[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        50, 50, 0, -10, -10, 0, 50, 50,
        25, 0, 0, 25, 25, 0, 0, 25,
        0, 0, 50, 90, 90, 50, 0, 0,
        25, 25, 25, 50, 50, 25, 25, 25,
        50, 50, 50, 90, 90, 50, 50, 50,
        90, 90, 90, 90, 90, 90, 90, 90,
        0, 0, 0, 0, 0, 0, 0, 0
};

int KnightTable[64] = {
        0, -50, 0, 0, 0, 0, -50, 0,
        0, 0, 0, 25, 25, 0, 0, 0,
        0, 0, 50, 50, 50, 50, 0, 0,
        0, 25, 50, 90, 90, 50, 0, 0,
        0, 50, 70, 90, 90, 70, 0, 25,
        0, 50, 50, 90, 90, 50, 0, 25,
        0, 0, 25, 50, 50, 25, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
};

int BishopTable[64] = {
        0, 0, -50, 0, 0, -50, 0, 0,
        0, 60, 0, 50, 50, 30, 0, 0,
        0, 0, 50, 70, 70, 50, 0, 0,
        0, 50, 70, 90, 90, 70, 50, 0,
        0, 50, 70, 90, 90, 70, 50, 0,
        0, 0, 50, 70, 70, 50, 0, 0,
        0, 0, 0, 50, 50, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
};

int RookTable[64] = {
        0, 0, 70, 70, 60, 25, 0, 0,
        0, 0, 25, 50, 50, 25, 0, 0,
        0, 0, 25, 50, 50, 25, 0, 0,
        0, 0, 25, 50, 50, 25, 0, 0,
        0, 0, 25, 50, 50, 25, 0, 0,
        0, 0, 25, 50, 50, 25, 0, 0,
        90, 90, 90, 90, 90, 90, 90, 90,
        0, 0, 25, 50, 50, 25, 0, 0
};

//// l'evaluation du roi pendant la fin de partie
int KingE[64] = {
        -500, -50, 0, 0, 0, 0, -50, -500,
        -50, 0, 50, 50, 50, 50, 0, -50,
        0, 50, 90, 90, 90, 90, 50, 0,
        0, 50, 90, 99, 99, 90, 50, 0,
        0, 50, 90, 99, 99, 90, 50, 0,
        0, 50, 90, 90, 90, 90, 50, 0,
        -50, 0, 50, 50, 50, 50, 0, -50,
        -500, -50, 0, 0, 0, 0, -50, -500
};

//// L'evaluation du roi pendant l'ouverture, on donne la priorite au petit roque (plus sucurise)
int KingO[64] = {
        0, 50, 12, -12, -50, 40, 50, 25,
        -300, -300, -300, -300, -300, -300, -300, -300,
        -500, -500, -500, -500, -500, -500, -500, -500,
        -700, -700, -700, -700, -700, -700, -700, -700,
        -700, -700, -700, -700, -700, -700, -700, -700,
        -700, -700, -700, -700, -700, -700, -700, -700,
        -700, -700, -700, -700, -700, -700, -700, -700,
        -700, -700, -700, -700, -700, -700, -700, -700
};

// Compter le nombre des piéces
int nbrPieces(struct config board, bool type);

int estim(struct config board);


int Calculer_avantage_pendule(int Cadence) {
    int res = 0;
    if (Cadence == 1) res = 80;
    else if (Cadence == 2) res = 150;
    else if (Cadence == 3) res = 10;
    return res;
}

int nbrPieces(struct config board, bool type) {
    int i, j, nbr = 0;
    if (type) {
        for (i = 0; i <= 7; i++) {
            for (j = 0; j <= 7; j++) {
                if (board.mat[i][j] > 0) {
                    nbr++;
                }
            }
        }
    } else {
        for (i = 0; i <= 7; i++) {
            for (j = 0; j <= 7; j++) {
                if (board.mat[i][j] < 0) {
                    nbr++;
                }
            }
        }
    }
    return nbr;
}

int estim(struct config board) /// La fonction d'evaluation de la configuration
{
    int i, j;
    int matrice = 0;
    int isole = 0, rowB, rowN, nbrPionB = 0, nbrPionN = 0, doubl = 0;
    bool pionPosB_1 = false, pionPosB_2 = false, pionPosB = false;
    bool pionPosN_1 = false, pionPosN_2 = false, pionPosN = false;
    int k, rockB_nbrOpen = 0, rockN_nbrOpen = 0, rockValue = 0;
    int queenN_nbrOpen = 0, queenB_nbrOpen = 0, queenValue = 0;
    int nbrBishopB = 0, nbrBishopN = 0, bishopValue = 0;
    int results, materiel = 0;
    for (j = 0; j <= 7; j++) {
        for (i = 0; i <= 7; i++) {
            switch (board.mat[i][j]) {
                case 'p':
                    materiel += 100;

                    matrice += PawnTable[j + i * 8];

                    nbrPionB++;
                    pionPosB = true;
                    if (!pionPosB_1) {
                        if (!pionPosB_2) {
                            pionPosB_2 = true;
                            rowB = j;
                        } else {
                            if (rowB != j) {
                                pionPosB_1 = true;
                            }
                        }
                    }
                    bool pion_passe_exist_b = true;
                    bool pion_passe_connecte_exist_b = true;
                    for (int l = j; l < 8; ++l) {
                        if (board.mat[i][l] == -'p') pion_passe_exist_b = false;
                    }
                    if (pion_passe_exist_b) {
                        for (int l = j; l < 8; ++l) {
                            if (board.mat[i - 1][l] == -'p' || board.mat[i + 1][l] == -'p')
                                pion_passe_connecte_exist_b = false;
                        }
                        Pion_passe = +80;
                    }
                    if (pion_passe_connecte_exist_b)Pion_passe_connecte = +200;
                    break;
                case -'p':
                    materiel -= 100;

                    matrice -= PawnTable[j + (7 - i) * 8];

                    nbrPionN++;
                    pionPosN = true;
                    if (!pionPosN_1) {
                        if (!pionPosN_2) {
                            pionPosN_2 = true;
                            rowN = j;
                        } else {
                            if (rowN != j) {
                                pionPosN_1 = true;
                            }
                        }
                    }
                    bool pion_passe_exist_n = true;
                    bool pion_passe_connecte_exist_n = true;
                    for (int l = j; l > 0; --l) {
                        if (board.mat[i][l] == 'p') pion_passe_exist_n = false;
                    }
                    if (pion_passe_exist_n) {
                        for (int l = 0; l < 8; ++l) {
                            if (board.mat[i - 1][l] == 'p' || board.mat[i + 1][l] == 'p')
                                pion_passe_connecte_exist_b = false;
                        }
                        Pion_passe = -80;
                    }
                    if (pion_passe_connecte_exist_n)Pion_passe_connecte = -200;
                    break;
                case 'C':
                    materiel += 300;

                    matrice += KnightTable[j + i * 8];
                    break;
                case -'C':
                    materiel -= 300;

                    matrice -= KnightTable[j + (7 - i) * 8];
                    break;
                case 'f':
                    materiel += 325;

                    matrice += BishopTable[j + i * 8];

                    nbrBishopB++;
                    break;
                case -'f':
                    materiel -= 325;

                    matrice -= BishopTable[j + (7 - i) * 8];

                    nbrBishopN++;
                    break;
                case 't':
                    materiel += 500;

                    matrice += RookTable[j + i * 8];

                    k = 0;
                    while ((k <= 7) && (board.mat[k][j] != 'p')) {
                        if (((board.mat[k][j] == 0) || (board.mat[k][j] == 't')) || (board.mat[k][j] < 0)) {
                            rockB_nbrOpen++;
                        }
                        k++;
                    }
                    break;
                case -'t':
                    materiel -= 500;

                    matrice -= RookTable[j + (7 - i) * 8];

                    k = 7;
                    while ((k >= 0) && (board.mat[k][j] != -'p')) {
                        if (((board.mat[k][j] == 0) || (board.mat[k][j] == -'t')) || (board.mat[k][j] < 0)) {
                            rockN_nbrOpen++;
                        }
                        k--;
                    }
                    break;
                case 'n':
                    materiel += 1000;
                    k = 0;
                    while ((k <= 7) && (board.mat[k][j] != 'p')) {
                        if (((board.mat[k][j] == 0) || (board.mat[k][j] == 'n')) || (board.mat[k][j] < 0)) {
                            queenB_nbrOpen++;
                        }
                        k++;
                    }
                    if (nbrPieces(board, true) < 6 && nbrPieces(board, false) < 6 && board.mat[i][j] == 'r' &&
                        (board.mat[i + 2][j - 2] == 'r' || board.mat[i + 2][j] == 'r' || board.mat[i + 2][j + 2] == 'r'
                         || board.mat[i][j + 2] == 'r' || board.mat[i][j - 2] == 'r')) {
                        Echec_perpetual = +20;
                    }
                    break;
                case -'n':
                    materiel -= 1000;
                    if (board.mat[i + 1][j] == 'r' || board.mat[i + 1][j + 1] == 'r' || board.mat[i][j + 1] == 'r'
                        || board.mat[i - 1][j + 1] == 'r' || board.mat[i - 1][j] == 'r' ||
                        board.mat[i - 1][j - 1] == 'r') {
                        printf("here i==%d j==%d\n", i, j);

                    }
                    k = 7;
                    while ((k >= 0) && (board.mat[k][j] != -'p')) {
                        if (((board.mat[k][j] == 0) || (board.mat[k][j] == -'n')) || (board.mat[k][j] < 0)) {
                            queenN_nbrOpen++;
                        }
                        k--;
                    }
                    if (nbrPieces(board, true) < 6 && nbrPieces(board, false) < 6 && board.mat[i][j] == 'r' &&
                        (board.mat[i + 2][j - 2] == 'r' || board.mat[i + 2][j] == 'r' || board.mat[i + 2][j + 2] == 'r'
                         || board.mat[i][j + 2] == 'r' || board.mat[i][j - 2] == 'r')) {
                        Echec_perpetual = -20;
                    }
                    break;
                case 'r':
                    materiel += 1500;
                    if (nbrPieces(board, true) > 8) {
                        matrice += KingO[j + i * 8];
                    }
                    if (nbrPieces(board, true) < 7) {
                        matrice += KingE[j + i * 8];
                    }
                    if (i < 8 && j < 8 && j > 0) {
                        if (board.mat[i + 1][j] == 'p' && board.mat[i + 1][j - 1] == 'p' &&
                            board.mat[i + 1][j + 1] == 'p') {
                            Securite_du_roi = +10;
                        }
                    }
                    if (i == 0 && j == 1) {//le petit roque pour les blanc
                        Roque = 25;
                    } else if (i == 0 && j == 5) {//le grand roque pour les blanc
                        Roque = 24;
                    }
                    break;
                case -'r':
                    materiel -= 1500;
                    if (nbrPieces(board, true) > 8) {
                        matrice -= KingO[j + (7 - i) * 8];
                    }
                    if (nbrPieces(board, true) < 7) {
                        matrice -= KingE[j + (7 - i) * 8];
                    }
                    if (i > 0 && j < 8 && j > 0) {
                        if (board.mat[i - 1][j] == 'p' && board.mat[i + 1][j - 1] == 'p' &&
                            board.mat[i + 1][j + 1] == 'p') {
                            Securite_du_roi = -10;
                        }
                    }
                    if (i == 7 && j == 1) {//le petit roque pour les noires
                        Roque = -25;
                    } else if (i == 7 && j == 5) {//le grand roque pour les noires
                        Roque = -24;
                    }
                    break;
            }
        }

        if (nbrPionB > 0) {
            doubl = doubl + nbrPionB - 1;
        }
        if (nbrPionN > 0) {
            doubl = doubl + nbrPionN - 1;
        }
        nbrPionB = 0;
        nbrPionN = 0;

        if (!pionPosB && !pionPosB_1 && pionPosB_2) {
            isole++;
        }
        if (!pionPosB) {
            pionPosB_1 = false;
            pionPosB_2 = false;
        }
        pionPosB = false;

        nbrPionN = 0;
        if (!pionPosN && !pionPosN_1 && pionPosN_2) {
            isole--;
        }
        if (!pionPosN) {
            pionPosN_1 = false;
            pionPosN_2 = false;
        }
        pionPosN = false;

        if (rockB_nbrOpen == 8) {
            rockValue += Tour_colonne_ouverte;
        } else {
            if (rockB_nbrOpen > 5) {
                rockValue += Tour_colonne_semi_ouverte;
            }
        }
        if (rockN_nbrOpen == 8) {
            rockValue -= Tour_colonne_ouverte;
        } else {
            if (rockN_nbrOpen > 5) {
                rockValue -= Tour_colonne_semi_ouverte;
            }
        }
        rockB_nbrOpen = 0;
        rockN_nbrOpen = 0;

        if (queenB_nbrOpen == 8) {
            queenValue += Dame_colonne_ouverte;
        } else {
            if (queenB_nbrOpen > 5) {
                queenValue += Dame_colonne_semi_ouverte;
            }
        }
        if (queenN_nbrOpen == 8) {
            queenValue -= Dame_colonne_ouverte;
        } else {
            if (queenN_nbrOpen > 5) {
                queenValue -= Dame_colonne_semi_ouverte;
            }
        }
        queenB_nbrOpen = 0;
        queenN_nbrOpen = 0;

        if (nbrBishopB == 2) {
            bishopValue += Paire_de_fous;
            nbrBishopB = 0;
        }
        if (nbrBishopN == 2) {
            bishopValue -= Paire_de_fous;
            nbrBishopN = 0;
        }
    }
    Avantage_pendule = Calculer_avantage_pendule(Cadence);
    results = materiel + doubl * Pion_double + isole * Pion_isole + Echec_perpetual +
              rockValue + queenValue + bishopValue + matrice + Pion_passe_connecte +
              Pion_passe + Avantage_pendule + Roque;
    //retourner une valeur dans ]-100 , +100[
    int final_res = results / 40;
    if (final_res > 100) return 100;
    if (final_res < -100) return -100;

    return final_res;
};

// estim


