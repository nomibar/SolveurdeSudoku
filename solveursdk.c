#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define N 3 //taille d'un bloc
#define TAILLE N*N //taille de la grille




typedef struct {
    int valeur;
    int candidats[TAILLE]; // la case d’indice 0
    int nbCandidats;
} tCase1;
typedef tCase1 tGrille[TAILLE][TAILLE];


tGrille grille1;


void chargerGrille(tGrille g,char nomFichier[30]){

     FILE * f;

    f = fopen(nomFichier, "rb");
    if (f==NULL){
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    } 
    else{
        for (int i = 0; i < TAILLE; i++) {
            for (int j = 0; j < TAILLE; j++) {
                fread(&g[i][j].valeur, sizeof(int), 1, f);
                //lis les valeurs de la grille
                
                if(g[i][j].valeur==0){
                    for(int k=0;k<TAILLE;k++){
                        g[i][j].candidats[k]=k+1;
                    }
                    g[i][j].nbCandidats = 9;
                    //si la valeur est 0 on met le nombre de candidat a 9 et on ajout tous les candidats
                }
                else{
                    for (int k = 0; k < TAILLE; k++) {
                        g[i][j].candidats[k] = 0;
                    }
                    g[i][j].nbCandidats = 0;
                    // si la valeur n'est pas 0 on met tout les candidats a 0 et le nombre de candidat a 0
                }  
            }
        }
    }
    fclose(f);
}

void supprimerCandidatCase(tGrille g,int ligne,int colonne){
    //supprime les candidats dans le block
    int valligne,valcol;
    int resteligne,restecol;
    resteligne=ligne%N;
    restecol=colonne%N;
    valligne=ligne-resteligne;
    valcol=colonne-restecol;
    //calcule le reste et le soustrait a la ligne et colonne afin d'acceder a l'indice le plus en haut a gauche du block pour pouvoir parcourir le block apres
    for(int i=0;i<N;i++){
        for(int k=0;k<N;k++){
            if(g[valligne+i][valcol+k].candidats[g[ligne][colonne].valeur -1]!=0){
                g[valligne+i][valcol+k].candidats[g[ligne][colonne].valeur -1]=0;
                g[valligne+i][valcol+k].nbCandidats=g[valligne+i][valcol+k].nbCandidats-1;
            }
        }
    }
    //parcour le block et supprime les candidats correspondant a la valeur de la case ligne colonne

}

void supprimerCandidatLigneColonne(tGrille g,int ligne,int colonne){
    for(int k=0;k<TAILLE;k++){
        if(g[ligne][k].candidats[g[ligne][colonne].valeur -1]!=0){
            g[ligne][k].candidats[g[ligne][colonne].valeur -1]=0;
            g[ligne][k].nbCandidats=g[ligne][k].nbCandidats-1;  
            
        }
        if(g[k][colonne].candidats[g[ligne][colonne].valeur -1]!=0){
            g[k][colonne].candidats[g[ligne][colonne].valeur -1]=0;
            g[k][colonne].nbCandidats=g[k][colonne].nbCandidats-1;
            
        }
    }
    //parcours la ligne et la colonne depuis la case ligne colonne et supprime tout les candidats correspondant 

}




void initialiserGrille(tGrille g){
    //initaialise la grille est fait appelle a 2 procedure pour supprimer les candidats
    int ligne;
    int colonne;
    //int k;
    for(ligne=0;ligne<TAILLE;ligne++){
        for(colonne=0;colonne<TAILLE;colonne++){
            if(g[ligne][colonne].valeur!=0){
                supprimerCandidatLigneColonne(g,ligne,colonne);
                supprimerCandidatCase(g,ligne,colonne);
            }
        }
    }
}

void afficherGrille(tGrille t){
    printf("    1  2  3   4  5  6   7  8  9 \n");
    int ligne; // indice de ligne
    int colonne; // indice de colonne
    for (ligne=0 ; ligne<TAILLE ; ligne++){
        if (ligne%N==0){
            printf("  +---------+---------+---------+\n");
        }
        printf("%d |",ligne+1);
        for(colonne=0 ; colonne<TAILLE ; colonne++){
            // acces a chaque case t[ligne][colonne] du tableau
            if (colonne%N==0 && colonne!=0){
                printf("|");//tout les N valeur affiche un | pour la formatation
            }
            if (t[ligne][colonne].valeur==0){
                printf(" . ");//affiche un . quand la case est vide 
            }
            
            else{
                printf(" %d ",t[ligne][colonne].valeur);//affiche la case du tableau correspondante
            }
        }
        printf("|\n");
            
    }
    printf("  +---------+---------+---------+\n");
    
}


void afficherGrille1(tGrille g) {
    //affiche la grille affin de voir la valeur les indice le nombre de candidat et les valeur de chaque candidat
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            printf("ligne : %d colonne : %d\tValeur : %d\tCandidats : [",i,j, g[i][j].valeur);

            for (int k = 0; k < TAILLE; k++) {
                printf("%d", g[i][j].candidats[k]);

                if (k < TAILLE - 1) {
                    printf(", ");
                }
            }

            printf("]\tNombre de candidats : %d\n", g[i][j].nbCandidats);
        }
    }
}

int nbCasesVidesf(tGrille g,int nbCasesVides){
    //parcours le tableau et diminue de 1 le nombre de case vide pour chaque valeur differante de 0 rencontré
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
    //parcours le tableau et diminue de 1 le nombre de case vide pour chaque valeur differante de 0 rencontré
    int ligne;
    int colonne;
    for ( ligne= 0; ligne < TAILLE; ligne++) {
        for ( colonne = 0; colonne < TAILLE; colonne++) {
            nbCand=nbCand+g[ligne][colonne].nbCandidats;
            }
        }
    return nbCand;
}
int caseremplie(tGrille g){
    int res;
    res=0;
    for ( int ligne= 0; ligne < TAILLE; ligne++) {
        for ( int colonne = 0; colonne < TAILLE; colonne++) {
            if(g[ligne][colonne].valeur!=0){
                res++;
            }
            }
        }
    return res;
}
void affichagefinal(tGrille g,int nbCasesVides,char nomFichier[30],int nbCand,int nbCandtot){
    int caseTotale;
    int nbCaseRemplie;
    float tauxDeRemplisage;
    int nbcandelim;
    float tauxelim;
    
    caseTotale=TAILLE*TAILLE;
    
    nbCaseRemplie=caseremplie(g);
    
    tauxDeRemplisage=(nbCaseRemplie*100)/caseTotale;
    
    nbcandelim=nbCandtot-nbCand;

    tauxelim=(nbcandelim*100)/nbCandtot;

    printf("*****   RESULTATS POUR %s *****\n",nomFichier);
    printf("Nombre de case remplies = %d sur %d \t Taux de remplisssage = %.3f %% \n",nbCaseRemplie,caseTotale,tauxDeRemplisage);
    printf("Nombre de candidats elimines = %d sur %d \t pourcentage d'elimination = %.3f %% \n",nbcandelim,nbCandtot,tauxelim);
}

void singletoncachelignecolonneblock(tGrille g,int ligne,int colonne,bool *progression){
    bool present;
    for (int k = 0; k < TAILLE; k++) {
        
        int i = 0;
        present = true; 
        while (i < TAILLE && present) {
            if ((g[ligne][colonne].candidats[k] == g[ligne][i].candidats[k] && i != colonne) || g[ligne][colonne].candidats[k]==0) {
                present = false;
                                    
            }
            i++;
        }// Si un singleton caché est trouvé dans la ligne, met à jour la grille

        if (present) {
            //retire les candidat de la colonne
            g[ligne][colonne].valeur = g[ligne][colonne].candidats[k];
            for (int j = 0; j < TAILLE; j++) {
                g[ligne][colonne].candidats[j] = 0;
                if(g[j][colonne].candidats[k]!=0){
                    g[j][colonne].candidats[k]=0;
                    g[j][colonne].nbCandidats = g[j][colonne].nbCandidats-1;
                }

            }
            //retire les candidats du block
            int blockligne = (ligne / N) * N;
            int blockcolonne = (colonne / N) * N;
            for (int i = blockligne; i < blockligne + N; i++) {
                for (int j = blockcolonne; j < blockcolonne + N; j++) {
                    if (g[i][j].candidats[k] != 0) {
                        g[i][j].candidats[k] = 0;
                        g[i][j].nbCandidats = g[i][j].nbCandidats - 1;
                    }
                }
            }

            g[ligne][colonne].nbCandidats = 0;
            *progression = true;
        }
       
        
        if(*progression==false){
            i = 0;
            present = true; 
            while (i < TAILLE && present) {
                if ((g[ligne][colonne].candidats[k] == g[i][colonne].candidats[k] && i != ligne) || g[ligne][colonne].candidats[k]==0) {
                    present = false;
                }
                i++;
            }// Si un singleton caché est trouvé dans la colonne, met à jour la grille
            
            if (present) {
                //retire les candidat de la colonne
                g[ligne][colonne].valeur = g[ligne][colonne].candidats[k];
                for (int j = 0; j < TAILLE; j++) {
                    g[ligne][colonne].candidats[j] = 0;
                    if(g[ligne][j].candidats[k]!=0){
                        g[ligne][j].candidats[k]=0;
                        g[ligne][j].nbCandidats = g[ligne][j].nbCandidats-1;
                    }   
                }
                //retire les candidat du block
                int blockligne = (ligne / N) * N;
                int blockcolonne = (colonne / N) * N;
                for (int i = blockligne; i < blockligne + N; i++) {
                    for (int j = blockcolonne; j < blockcolonne + N; j++) {
                        if (g[i][j].candidats[k] != 0) {
                            g[i][j].candidats[k] = 0;
                            g[i][j].nbCandidats = g[i][j].nbCandidats - 1;
                        }
                    }
                }

                g[ligne][colonne].nbCandidats = 0;
                *progression = true;
            }
        }
        
        //verfie dans la collonne grace a la technique du singleton caché
        if (*progression == false) {
            // Calculez les indices du bloc
            int blockligne = (ligne / N) * N;
            int blockcolonne = (colonne / N) * N;
            //verifie le block
            for (int i = blockligne; i < blockligne + N; i++) {
                for (int j = blockcolonne; j < blockcolonne + N; j++) {
                    if ((g[ligne][colonne].candidats[k] == g[i][j].candidats[k] && (i != ligne || j != colonne)) || g[ligne][colonne].candidats[k] == 0) {
                        present = false;
                        break; // Pas besoin de continuer à vérifier si la condition est deja fausse
                    }
                }
            }

            if (present) {
                g[ligne][colonne].valeur = g[ligne][colonne].candidats[k];
                for (int i = blockligne; i < blockligne + N; i++) {
                    for (int j = blockcolonne; j < blockcolonne + N; j++) {
                        g[ligne][colonne].candidats[j] = 0;
                        if (g[i][j].candidats[k] != 0) {
                            g[i][j].candidats[k] = 0;
                            g[i][j].nbCandidats = g[i][j].nbCandidats - 1;
                        }
                    }
                }//retire dans la ligne et la colonne
                int blockligne = (ligne / N) * N;
                int blockcolonne = (colonne / N) * N;
                for (int i = blockligne; i < blockligne + N; i++) {
                    for (int j = blockcolonne; j < blockcolonne + N; j++) {
                        if (g[i][j].candidats[k] != 0) {
                            g[i][j].candidats[k] = 0;
                            g[i][j].nbCandidats = g[i][j].nbCandidats - 1;
                        }
                    }
                }//retire dans le block

                g[ligne][colonne].nbCandidats = 0;
                *progression = true;
                g[ligne][colonne].nbCandidats = 0;
                *progression = true;
             }

        }




    }

}

void pairesNues(tGrille g, int ligne, int colonne, bool *progression) {
    // Recherche des paires nues dans la ligne
    for (int i = 0; i < TAILLE; i++) {
        if (i != colonne && g[ligne][colonne].nbCandidats == 2 && g[ligne][i].nbCandidats == 2 && g[ligne][colonne].candidats[0] == g[ligne][i].candidats[0] && g[ligne][colonne].candidats[1] == g[ligne][i].candidats[1]) {
            // elimine les candidats de la ligne pour les autres colonnes
            
            for (int j = 0; j < TAILLE; j++) {
                if (j != colonne && g[ligne][j].nbCandidats == 2) {
                    for (int k = 0; k < 2; k++) {
                        int candidatARetirer = g[ligne][colonne].candidats[k];
                        if (g[ligne][j].candidats[candidatARetirer] != 0) {
                            g[ligne][j].candidats[candidatARetirer] = 0;
                            g[ligne][j].nbCandidats--;
                            *progression = true;
                        }
                    }
                }
            }
        }
    }

    // Recherche des paires dans la colonne et les élimine
    if (*progression == false) {
        for (int i = 0; i < TAILLE; i++) {
            if (i != ligne && g[ligne][colonne].nbCandidats == 2 && g[i][colonne].nbCandidats == 2 && g[ligne][colonne].candidats[0] == g[i][colonne].candidats[0] && g[ligne][colonne].candidats[1] == g[i][colonne].candidats[1]) {
                // elimine les candidats de la colonne pour les autres lignes
                
                for (int j = 0; j < TAILLE; j++) {
                    if (j != ligne && g[j][colonne].nbCandidats == 2) {
                        for (int k = 0; k < 2; k++) {
                            int candidatARetirer = g[ligne][colonne].candidats[k];
                            if (g[j][colonne].candidats[candidatARetirer] != 0) {
                                g[j][colonne].candidats[candidatARetirer ] = 0;
                                g[j][colonne].nbCandidats--;
                                *progression = true;
                                
                            }
                        }
                    }
                }
                // Sortir de la boucle apres avoir trouvé et traité une paire
                break;
                
            }
        }
    }

    // Recherche des paires dans le bloc et les élimine
    if (*progression == false) {
        int blockLigne = (ligne / N) * N;
        int blockColonne = (colonne / N) * N;

        for (int i = blockLigne; i < blockLigne + N; i++) {
            for (int j = blockColonne; j < blockColonne + N; j++) {
                if ((i != ligne || j != colonne) && g[ligne][colonne].nbCandidats == 2 && g[i][j].nbCandidats == 2 && g[ligne][colonne].candidats[0] == g[i][j].candidats[0] && g[ligne][colonne].candidats[1] == g[i][j].candidats[1]) {
                    // elimine les candidats du bloc
                    for (int k = blockLigne; k < blockLigne + N; k++) {
                        for (int l = blockColonne; l < blockColonne + N; l++) {
                            if ((k != ligne || l != colonne) && g[k][l].nbCandidats == 2) {
                                for (int m = 0; m < 2; m++) {
                                    int candidatARetirer = g[ligne][colonne].candidats[m];
                                    if (g[k][l].candidats[candidatARetirer] != 0) {
                                        g[k][l].candidats[candidatARetirer] = 0;
                                        g[k][l].nbCandidats--;
                                        *progression = true;
                                        
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}




void pairesCacheesBloc(tGrille g, int blockligne, int blockcolonne, bool *progression) {
    // Parcourt tous les candidats possibles
    for (int candidat1 = 1; candidat1 <= TAILLE - 1; candidat1++) {
        for (int candidat2 = candidat1 + 1; candidat2 <= TAILLE; candidat2++) {
            int count; 
            count=0;
            int premLigne,premColonne; 
            premLigne=-1;
            premColonne=-1;
            int secondLigne,secondColonne; 
            secondLigne=-1;
            secondColonne=-1;

            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    // verifie si les candidats sont présents dans la cellule
                    if (g[blockligne + i][blockcolonne + j].candidats[candidat1] != 0 &&
                        g[blockligne + i][blockcolonne + j].candidats[candidat2] != 0) {
                        count++;
                        // enregistre les cases 
                        if (count == 1) {
                            premLigne=blockligne+i;
                            premColonne=blockcolonne+j;
                        } else if (count == 2) {
                            secondLigne=blockligne+i;
                            secondColonne=blockcolonne+j;
                        }
                    }
                }
            }

            // Si une paire cachée est trouvée
            if (count == 2) {
                // elimine les candidats de toutes les autres cellules du bloc
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N; j++) {
                        if ((g[blockligne + i][blockcolonne + j].candidats[candidat1] != 0) &&
                            (g[blockligne + i][blockcolonne + j].candidats[candidat2] != 0) &&
                            ((blockligne + i != premLigne || blockcolonne + j != premColonne) != 0) &&
                            ((blockligne + i != secondLigne || blockcolonne + j != secondColonne) != 0)) {
                            // elimine les candidats et metaà jour le nombre de candidats
                            g[blockligne + i][blockcolonne + j].candidats[candidat1] = 0;
                            g[blockligne + i][blockcolonne + j].candidats[candidat2] = 0;
                            g[blockligne + i][blockcolonne + j].nbCandidats -= 2;
                            *progression = true;
                        }
                    }
                }
            }
        }
    }
}


void pairesCacheesLigne(tGrille g, int ligne, bool *progression) {
    // parcourt tous les candidats possibles
    for (int candidat1 = 1; candidat1 <= TAILLE - 1; candidat1++) {
        for (int candidat2 = candidat1 + 1; candidat2 <= TAILLE; candidat2++) {
            int count; 
            count= 0;
            int premColonne,secondColonne;
            premColonne=-1; 
            secondColonne =-1;

            for (int j = 0; j < TAILLE; j++) {
                // vérifie si les candidats sont présents dans la cellule
                if (g[ligne][j].candidats[candidat1] != 0 && g[ligne][j].candidats[candidat2] != 0) {
                    count++;
                    // enregistre les cases
                    if (count == 1) {
                        premColonne = j;
                    } else if (count == 2) {
                        secondColonne = j;
                    }
                }
            }

            // Si une paire cachée est trouvée
            if (count == 2) {
                // elimine les candidats de toutes les autres cellules de la ligne
                for (int j = 0; j < TAILLE; j++) {
                    if (g[ligne][j].candidats[candidat1] && g[ligne][j].candidats[candidat2] &&
                        j != premColonne && j != secondColonne) {
                        // elimine les candidats et met a jour le nombre de candidats
                        if (g[ligne][j].candidats[candidat1] != 0) {
                            g[ligne][j].candidats[candidat1] = 0;
                            g[ligne][j].nbCandidats = g[ligne][j].nbCandidats-1;
                        }
                        if (g[ligne][j].candidats[candidat2] != 0) {
                            g[ligne][j].candidats[candidat2] = 0;
                            g[ligne][j].nbCandidats = g[ligne][j].nbCandidats-1;
                        }
                        *progression = true;
                    }
                }
            }
        }
    }
}


void pairesCacheesColonne(tGrille g, int colonne, bool *progression) {
    // parcourt tous les candidats possibles
    for (int candidat1 = 1; candidat1 <= TAILLE - 1; candidat1++) {
        for (int candidat2 = candidat1 + 1; candidat2 <= TAILLE; candidat2++) {
            int count; 
            count= 0;
            int premLigne,secondLigne;
            premLigne=-1; 
            secondLigne =-1;

            
            for (int i = 0; i < TAILLE; i++) {
                // vérifie si les candidats sont présents dans la cellule
                if (g[i][colonne].candidats[candidat1] != 0 && g[i][colonne].candidats[candidat2] != 0) {
                    count++;
                    // enregistre les cases
                    if (count == 1) {
                        premLigne = i;
                    } else if (count == 2) {
                        secondLigne = i;
                    }
                }
            }

            // Si une paire cachée est trouvée
            if (count == 2) {
                // elimine les candidats de toutes les autres cellules de la colonne
                for (int i = 0; i < TAILLE; i++) {
                    if (g[i][colonne].candidats[candidat1] && g[i][colonne].candidats[candidat2] &&
                        i != premLigne && i != secondLigne) {
                        // elimine les candidats et met a jour le nombre de candidats
                        if (g[i][colonne].candidats[candidat1] != 0) {
                            g[i][colonne].candidats[candidat1]=0;
                            g[i][colonne].nbCandidats = g[i][colonne].nbCandidats -1;
                        }
                        if (g[i][colonne].candidats[candidat2] != 0) {
                            g[i][colonne].candidats[candidat2]=0;
                            g[i][colonne].nbCandidats = g[i][colonne].nbCandidats-1;
                        }
                        *progression = true;
                    }
                }
            }
        }
    }
}

void pairesCachees(tGrille g, bool *progression) {
    // Parcourt chaque ligne du Sudoku
    for (int ligne = 0; ligne < TAILLE; ligne++) {
        pairesCacheesLigne(g, ligne, progression);
    }

    // Si aucune progression n'a été réalisée, on parcourt chaque colonne du Sudoku
    if (*progression == false) {
        for (int colonne = 0; colonne < TAILLE; colonne++) {
            pairesCacheesColonne(g, colonne, progression);
        }
    }

    // Si aucune progression n'a été réalisée, on parcourt chaque bloc du Sudoku
    if (*progression == false) {
        for (int blockLigne = 0; blockLigne < TAILLE; blockLigne += N) {
            for (int blockColonne = 0; blockColonne < TAILLE; blockColonne += N) {
                pairesCacheesBloc(g, blockLigne, blockColonne, progression);
            }
        }
    }
}









int main(){
    tGrille g; 
    
    char nomfic[30];
    printf("Nom du fichier ? ");
    scanf("%s", nomfic); 

    bool progression;
    int nbCasesVides;

    //Chargement de la grille depuis le fichier
    chargerGrille(g, nomfic);

    int nbCandidatstotal;
    nbCandidatstotal = 0;

    //Calcul du nombre total de candidats dans la grille
    nbCandidatstotal = nombreCandidatsf(g, nbCandidatstotal);

    //Initialisation de la grille
    initialiserGrille(g);
    clock_t begin = clock();
    nbCasesVides = TAILLE * TAILLE;

    //Calcul du nombre initial de cases vides dans la grille
    nbCasesVides = nbCasesVidesf(g, nbCasesVides);

    // Affichage de la grille initiale
    afficherGrille(g);

    progression = true;


    while (nbCasesVides != 0 && progression) {
        progression = false;

        // Parcours de la grille pour remplir les cases avec un seul candidat
        for (int ligne = 0; ligne < TAILLE; ligne++) {
            for (int colonne = 0; colonne < TAILLE; colonne++) {
                if (g[ligne][colonne].valeur == 0) {
                    if (g[ligne][colonne].nbCandidats == 1) {
                        int i;
                        for (i = 0; i < TAILLE; i++) {
                            if (g[ligne][colonne].candidats[i] != 0) {
                                g[ligne][colonne].valeur = g[ligne][colonne].candidats[i];
                                g[ligne][colonne].candidats[i] = 0;
                                g[ligne][colonne].nbCandidats = 0;
                            }
                        }

                        nbCasesVides = nbCasesVides - 1;
                        supprimerCandidatLigneColonne(g, ligne, colonne);
                        supprimerCandidatCase(g, ligne, colonne);

                        progression = true;
                    }
                }
            }
        }
        

        //technique du singleton caché
        if (progression == false) {
            for (int ligne = 0; ligne < TAILLE; ligne++) {
                for (int colonne = 0; colonne < TAILLE; colonne++) {
                    if (g[ligne][colonne].valeur == 0) {
                        singletoncachelignecolonneblock(g, ligne, colonne, &progression);
                    }
                }
            }
        }
        
        //technique du singleton caché
        if (progression == false) {
            for (int ligne = 0; ligne < TAILLE; ligne++) {
                for (int colonne = 0; colonne < TAILLE; colonne++) {
                    pairesNues(g, ligne, colonne, &progression);
                }
            }
        }

        if (progression == false){
            pairesCachees(g, &progression);
        }
        
        
        
    }

    // Affichage de la grille résolue
    
    afficherGrille(g);

    int nombreDeCandidats;
    nombreDeCandidats = 0;

    // Calcul du nombre final de candidats dans la grille
    nombreDeCandidats = nombreCandidatsf(g, nombreDeCandidats);

    // Affichage des résultats finaux
    affichagefinal(g,nbCasesVides, nomfic, nombreDeCandidats, nbCandidatstotal);
    clock_t end = clock();
    double tmpsCPU = (end - begin)*1.0 / CLOCKS_PER_SEC;
    printf( "Temps CPU = %.3f secondes\n",tmpsCPU);
}
