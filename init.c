//
// Created by mahamdi on 1/30/19.
//


/* Intialise la disposition des pieces dans la configuration initiale conf */
void init(struct config *conf) {
    //|__|__|F|Re|Ro|T|__|__|

    int i, j;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            conf->mat[i][j] = 0;    // Les cases vides sont initialisées avec 0
    //les blans
    conf->mat[0][2] = 'f';
    conf->mat[0][3] = 'n';
    conf->mat[0][4] = 'r';
    conf->mat[0][5] = 't';
    //les noires
    for (j = 0; j < 8; j++) {
        conf->mat[7][j] = -conf->mat[0][j];
    }
    //
    conf->xrB = 0;
    conf->yrB = 4;
    conf->xrN = 7;
    conf->yrN = 4;

    conf->roqueB = 'n';//le grand roque non réalisable pour les blans
    conf->roqueN = 'n';//le grand roque non réalisable pour les noires


    conf->val = 0;

} // init

