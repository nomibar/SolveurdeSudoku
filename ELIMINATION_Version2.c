#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 3 // Taille d'un bloc
#define TAILLE N * N // Taille de la grille

typedef struct {
    int valeur;
    bool candidats[TAILLE + 1];
    int nbCandidats;
} tCase2;

typedef tCase2 tGrille[TAILLE][TAILLE];

void chargerGrille(tGrille g, char nomFichier[30]) {
    FILE *f;

    f = fopen(nomFichier, "rb");
    if (f == NULL) {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    } else {
        for (int i = 0; i < TAILLE; i++) {
            for (int j = 0; j < TAILLE; j++) {
                fread(&g[i][j].valeur, sizeof(int), 1, f);

                if (g[i][j].valeur == 0) {
                    for (int k = 1; k <= TAILLE; k++) {
                        g[i][j].candidats[k] = true;
                    }
                    g[i][j].nbCandidats = TAILLE;
                } else {
                    for (int k = 1; k <= TAILLE; k++) {
                        g[i][j].candidats[k] = false;
                    }
                    g[i][j].candidats[g[i][j].valeur] = true;
                    g[i][j].nbCandidats = 0;
                }
            }
        }
    }
    fclose(f);
}

void supprimerCandidatCase(tGrille g, int ligne, int colonne) {
    int valligne, valcol;
    int resteligne, restecol;
    resteligne = ligne % N;
    restecol = colonne % N;
    valligne = ligne - resteligne;
    valcol = colonne - restecol;

    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            if (g[valligne + i][valcol + k].candidats[g[ligne][colonne].valeur]) {
                g[valligne + i][valcol + k].candidats[g[ligne][colonne].valeur] = false;
                g[valligne + i][valcol + k].nbCandidats = g[valligne + i][valcol + k].nbCandidats - 1;
            }
        }
    }
}

void supprimerCandidatLigneColonne(tGrille g, int ligne, int colonne) {
    for (int k = 1; k <= TAILLE; k++) {
        if (g[ligne][k].candidats[g[ligne][colonne].valeur]) {
            g[ligne][k].candidats[g[ligne][colonne].valeur] = false;
            g[ligne][k].nbCandidats = g[ligne][k].nbCandidats - 1;
        }
        if (g[k][colonne].candidats[g[ligne][colonne].valeur]) {
            g[k][colonne].candidats[g[ligne][colonne].valeur] = false;
            g[k][colonne].nbCandidats = g[k][colonne].nbCandidats - 1;
        }
    }
}

void initialiserGrille(tGrille g) {
    int ligne;
    int colonne;
    int k;
    for (ligne = 0; ligne < TAILLE; ligne++) {
        for (colonne = 0; colonne < TAILLE; colonne++) {
            if (g[ligne][colonne].valeur != 0) {
                supprimerCandidatLigneColonne(g, ligne, colonne);
                supprimerCandidatCase(g, ligne, colonne);
            }
        }
    }
}

void afficherGrille(tGrille t){
    printf("    1  2  3   4  5  6   7  8  9 \n");
    int ligne;
    int colonne;
    for (ligne=0 ; ligne<TAILLE ; ligne++){
        if (ligne%N==0){
            printf("  +---------+---------+---------+\n");
        }
        printf("%d |",ligne+1);
        for(colonne=0 ; colonne<TAILLE ; colonne++){
            if (colonne%N==0 && colonne!=0){
                printf("|");
            }
            if (t[ligne][colonne].valeur==0){
                printf(" . ");
            }
            
            else{
                printf(" %d ",t[ligne][colonne].valeur);
            }
        }
        printf("|\n");  
    }
    printf("  +---------+---------+---------+\n");
}

int nbCasesVidesf(tGrille g,int nbCasesVides){
    int ligne;
    int colonne;
    for ( ligne= 0; ligne < TAILLE; ligne++) {
        for ( colonne = 0; colonne < TAILLE; colonne++) {
            if(g[ligne][colonne].valeur!=0){
                nbCasesVides=nbCasesVides-1;
            }
        }
    }
    return nbCasesVides;
}

int nombreCandidatsf(tGrille g,int nbCand){
    int ligne;
    int colonne;
    for ( ligne= 0; ligne < TAILLE; ligne++) {
        for ( colonne = 0; colonne < TAILLE; colonne++) {
            nbCand=nbCand+g[ligne][colonne].nbCandidats;
            }
        }
    return nbCand;
}

void afficherStats(tGrille g, char nomFichier[30]) {
    int caseTotale = TAILLE * TAILLE;
    int nbCaseRemplie = 0;
    int nbCandTot = 0;
    int nbCandElim = 0;

    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (g[i][j].valeur != 0) {
                nbCaseRemplie++;
            }

            nbCandTot += TAILLE;

            for (int k = 1; k <= TAILLE; k++) {
                if (g[i][j].candidats[k]) {
                    nbCandElim++;
                }
            }
        }
    }

    float tauxDeRemplissage = (float)nbCaseRemplie / caseTotale * 100;
    float tauxElim = (float)nbCandElim / nbCandTot * 100;

    printf("*****   RESULTATS POUR %s   *****\n", nomFichier);
    printf("Nombre de cases remplies : %d sur %d\n", nbCaseRemplie, caseTotale);
    printf("Taux de remplissage : %.3f %%\n", tauxDeRemplissage);
    printf("Nombre total de candidats éliminés : %d sur %d\n", nbCandElim, nbCandTot);
    printf("Pourcentage d'élimination : %.3f %%\n", tauxElim);
}

void singletonNu(tGrille g, int *nbCasesVides, bool *progression) {
    for (int ligne = 0; ligne < TAILLE; ligne++) {
        for (int colonne = 0; colonne < TAILLE; colonne++) {
            if (g[ligne][colonne].valeur == 0 && g[ligne][colonne].nbCandidats == 1) {
                for (int k = 0; k < TAILLE; k++) {
                    if (g[ligne][colonne].candidats[k]) {
                        g[ligne][colonne].valeur = k + 1;
                        g[ligne][colonne].candidats[k] = false;
                        g[ligne][colonne].nbCandidats = 0;
                        *nbCasesVides = *nbCasesVides - 1;
                        *progression = true;

                        for (int j = 0; j < TAILLE; j++) {
                            if (j != colonne && g[ligne][j].candidats[k]) {
                                g[ligne][j].candidats[k] = false;
                                g[ligne][j].nbCandidats--;
                            }
                        }

                        for (int i = 0; i < TAILLE; i++) {
                            if (i != ligne && g[i][colonne].candidats[k]) {
                                g[i][colonne].candidats[k] = false;
                                g[i][colonne].nbCandidats--;
                            }
                        }

                        int blockLigne = (ligne / N) * N;
                        int blockColonne = (colonne / N) * N;
                        for (int i = blockLigne; i < blockLigne + N; i++) {
                            for (int j = blockColonne; j < blockColonne + N; j++) {
                                if ((i != ligne || j != colonne) && g[i][j].candidats[k]) {
                                    g[i][j].candidats[k] = false;
                                    g[i][j].nbCandidats--;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void singletonCacheLigne(tGrille g, int ligne, int valeur) {
    for (int colonne = 0; colonne < TAILLE; colonne++) {
        if (g[ligne][colonne].valeur == 0 && g[ligne][colonne].candidats[valeur]) {
            g[ligne][colonne].valeur = valeur;
            g[ligne][colonne].nbCandidats = 0;

            for (int i = 0; i < TAILLE; i++) {
                if (i != ligne && g[i][colonne].candidats[valeur]) {
                    g[i][colonne].candidats[valeur] = false;
                    g[i][colonne].nbCandidats--;
                }
            }

            for (int j = 0; j < TAILLE; j++) {
                if (j != colonne && g[ligne][j].candidats[valeur]) {
                    g[ligne][j].candidats[valeur] = false;
                    g[ligne][j].nbCandidats--;
                }
            }
        }
    }
}

void singletonCacheColonne(tGrille g, int colonne, int valeur) {
    for (int ligne = 0; ligne < TAILLE; ligne++) {
        if (g[ligne][colonne].valeur == 0 && g[ligne][colonne].candidats[valeur]) {

            g[ligne][colonne].valeur = valeur;
            g[ligne][colonne].nbCandidats = 0;

            for (int j = 0; j < TAILLE; j++) {
                if (j != colonne && g[ligne][j].candidats[valeur]) {
                    g[ligne][j].candidats[valeur] = false;
                    g[ligne][j].nbCandidats--;
                }
            }

            for (int i = 0; i < TAILLE; i++) {
                if (i != ligne && g[i][colonne].candidats[valeur]) {
                    g[i][colonne].candidats[valeur] = false;
                    g[i][colonne].nbCandidats--;
                }
            }
        }
    }
}

void singletonCacheBloc(tGrille g, int blockLigne, int blockColonne, int valeur) {
    for (int i = blockLigne; i < blockLigne + N; i++) {
        for (int j = blockColonne; j < blockColonne + N; j++) {
            if (g[i][j].valeur == 0 && g[i][j].candidats[valeur]) {
                
                g[i][j].valeur = valeur;
                g[i][j].nbCandidats = 0;

                for (int k = blockLigne; k < blockLigne + N; k++) {
                    for (int l = blockColonne; l < blockColonne + N; l++) {
                        if ((k != i || l != j) && g[k][l].candidats[valeur]) {
                            g[k][l].candidats[valeur] = false;
                            g[k][l].nbCandidats--;
                        }
                    }
                }

                for (int k = 0; k < TAILLE; k++) {
                    if (k < blockLigne || k >= blockLigne + N) {
                        if (g[k][j].candidats[valeur]) {
                            g[k][j].candidats[valeur] = false;
                            g[k][j].nbCandidats--;
                        }
                    }
                }

                for (int l = 0; l < TAILLE; l++) {
                    if (l < blockColonne || l >= blockColonne + N) {
                        if (g[i][l].candidats[valeur]) {
                            g[i][l].candidats[valeur] = false;
                            g[i][l].nbCandidats--;
                        }
                    }
                }
            }
        }
    }
}

void singletonCache(tGrille g) {
    for (int valeur = 1; valeur <= TAILLE; valeur++) {
        
        for (int blockLigne = 0; blockLigne < TAILLE; blockLigne += N) {
            for (int blockColonne = 0; blockColonne < TAILLE; blockColonne += N) {
                singletonCacheBloc(g, blockLigne, blockColonne, valeur);
            }
        }

        for (int ligne = 0; ligne < TAILLE; ligne++) {
            singletonCacheLigne(g, ligne, valeur);
        }

        for (int colonne = 0; colonne < TAILLE; colonne++) {
            singletonCacheColonne(g, colonne, valeur);
        }
    }
}

void pairesNuesBloc(tGrille g, int blockligne, int blockcolonne, bool *progression) {
    for (int candidat = 1; candidat <= TAILLE; candidat++) {
        int count = 0;
        int premLigne = -1, premColonne = -1;
        int secondLigne = -1, secondColonne = -1;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (g[blockligne + i][blockcolonne + j].candidats[candidat]) {
                    count++;
                    if (count == 1) {
                        premLigne = blockligne + i;
                        premColonne = blockcolonne + j;
                    } else if (count == 2) {
                        secondLigne = blockligne + i;
                        secondColonne = blockcolonne + j;
                    }
                }
            }
        }

        if (count == 2) {
            for (int i = 0; i < TAILLE; i++) {
                if (i != premLigne && i != secondLigne) {
                    if (g[i][premColonne].candidats[candidat]) {
                        g[i][premColonne].candidats[candidat] = false;
                        g[i][premColonne].nbCandidats--;
                        *progression = true;
                    }
                    if (g[i][secondColonne].candidats[candidat]) {
                        g[i][secondColonne].candidats[candidat] = false;
                        g[i][secondColonne].nbCandidats--;
                        *progression = true;
                    }
                }
            }
        }
    }
}

void pairesNues(tGrille g, bool *progression) {
    for (int blockligne = 0; blockligne < TAILLE; blockligne += N) {
        for (int blockcolonne = 0; blockcolonne < TAILLE; blockcolonne += N) {
            pairesNuesBloc(g, blockligne, blockcolonne, progression);
        }
    }
}

void pairesCacheesBloc(tGrille g, int blockligne, int blockcolonne, bool *progression) {
    for (int candidat1 = 1; candidat1 <= TAILLE - 1; candidat1++) {
        for (int candidat2 = candidat1 + 1; candidat2 <= TAILLE; candidat2++) {
            int count = 0;
            int premLigne = -1, premColonne = -1;
            int secondLigne = -1, secondColonne = -1;

            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (g[blockligne + i][blockcolonne + j].candidats[candidat1] &&
                        g[blockligne + i][blockcolonne + j].candidats[candidat2]) {
                        count++;
                        if (count == 1) {
                            premLigne = blockligne + i;
                            premColonne = blockcolonne + j;
                        } else if (count == 2) {
                            secondLigne = blockligne + i;
                            secondColonne = blockcolonne + j;
                        }
                    }
                }
            }

            if (count == 2) {
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N; j++) {
                        if (g[blockligne + i][blockcolonne + j].candidats[candidat1] &&
                            g[blockligne + i][blockcolonne + j].candidats[candidat2] &&
                            (blockligne + i != premLigne || blockcolonne + j != premColonne) &&
                            (blockligne + i != secondLigne || blockcolonne + j != secondColonne)) {
                            g[blockligne + i][blockcolonne + j].candidats[candidat1] = false;
                            g[blockligne + i][blockcolonne + j].candidats[candidat2] = false;
                            g[blockligne + i][blockcolonne + j].nbCandidats -= 2;
                            *progression = true;
                        }
                    }
                }
            }
        }
    }
}

void pairesCachees(tGrille g, bool *progression) {
    for (int blockligne = 0; blockligne < TAILLE; blockligne += N) {
        for (int blockcolonne = 0; blockcolonne < TAILLE; blockcolonne += N) {
            pairesCacheesBloc(g, blockligne, blockcolonne, progression);
        }
    }
}

int main() {
    tGrille g;

    char nomfic[30];
    printf("Nom du fichier ? ");
    scanf("%s", nomfic);

    bool progression;
    int nbCasesVides;
    chargerGrille(g, nomfic);

    int nbCandidatstotal = 0;
    nbCandidatstotal = nombreCandidatsf(g, nbCandidatstotal);

    initialiserGrille(g);

    nbCasesVides = TAILLE * TAILLE;
    nbCasesVides = nbCasesVidesf(g, nbCasesVides);

    afficherGrille(g);

    progression = true;

    while(nbCasesVides!=0 && progression) {
        progression=false;

        singletonNu(g, &nbCasesVides, &progression);
       
        singletonCache(g);
        
        if (progression == false) {
            pairesNues(g, &progression);
        }
        
        if (progression == false) {
            pairesCachees(g, &progression);
        }
    }

    afficherGrille(g);

    int nombreDeCandidats = 0;
    nombreDeCandidats = nombreCandidatsf(g, nombreDeCandidats);

    afficherStats(g, nomfic);

    return 0;
}