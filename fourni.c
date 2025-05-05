#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // pour time()
#include <string.h>

//////////////////////////////////////////
// Constantes:

#define LINUX false

// Option pour tourner l’ echiquier lorsque le joueur noir est en train de jouer:
#define TOURNER_ECHIQUIER true

// Contenu d’une case vide de l’ echiquier:
#define CASE_VIDE ' '

// L’ echiquier sera mod elis e par un tableau 2D de caractères, de taille NxN.
#define N 8

// Nombre maximal de pièces d'une couleur pouvant être captur ees.
#define NB_MAX_CAPTURES 15

// Nombre maximal de coup possible pour une pièce. Ne pas modifier:
#define NB_MAX_DEPL (4*(N)-5)

// Encodage des couleurs d'une case de l' echiquier:
#define C_BLANC 0
#define C_NOIR  1
#define C_VIDE  2

// Encodage de l' etat en cours d'une partie:
#define NON_TERMINEE  0
#define DEFAITE_BLANC 1
#define DEFAITE_NOIR  2
#define PARTIE_NULLE  3

// Encodage des pièces par un caractère - majuscules pour
// le joueur blanc et minuscules pour le joueur noir:
// Tour:     'T' ou 't'
// Fou:      'F' ou 'f'
// Dame:     'D' ou 'd'
// Roi:      'R' ou 'r'
// Cavalier: 'C' ou 'c'
// Pion:     'P' ou 'p'

//////////////////////////////////////////
// Prototypes des fonctions fournies:

// Ne pas modifier les prototypes ci-dessous.
// Si l'on souhaite impl ementer des fonctionnalit es non exig ees, on pourra
// faire cela en ajoutant des variables globales et de nouvelles fonctions.

void effaceConsole(void);
void afficheCaseEnCouleur(char c, int couleurEnEchec);

//////////////////////////////////////////
// Prototypes des fonctions à impl ementer:

// Initialisation et affichage:
bool estDansGrille(int ligne, int colonne);
bool estCaseVide(const char grille[N][N], int ligne, int colonne);
void videGrille(char grille[N][N]);
void copieGrille(char copie[N][N], const char grille[N][N]);
void initialiseEchiquier(char grille[N][N]);
void afficheEchiquier(const char grille[N][N], int num_coup, const size_t temps_debut, int couleurActuelle);
void afficheTempsEcoule(const size_t tempsDebut, time_t tempsActuel);
void tournerEchiquier(char grille[N][N]);
void ecranDeFin();

// Gestion des couleurs, saisie d'un coup:
int couleurAdverse(int couleur);
int trouveCouleur(const char grille[N][N], int ligne, int colonne);
bool estMajuscule(char c); // OK
bool convertitEnCoordonnees(const char notation[2], int coordonnees[2]);
void saisieCoup(char grille[N][N], int couleurActuelle);

// Impl ementation du jeu d' echec:
void trouvePositionRoi(const char grille[N][N], int positionRoi[2], int couleur);
void realiseCoup(char grille[N][N], const int depart[2], const int arrivee[2]);
bool realiseCoupSiValide(char grille[N][N], const int depart[2], const int arrivee[2]);
bool estEnEchec(const char grille[N][N], int couleur);
bool estCoupValide(const char grille[N][N], const int depart[2], const int arrivee[2]);
bool estDeplacementValide(const char grille[N][N], const int depart[2], const int arrivee[2]);
int partieTerminee(const char grille[N][N], int couleur);
int listeDeplacementsValides(const char grille[N][N], const int depart[2], int deplacements[NB_MAX_DEPL][2]);

// D eplacements g en eriques
int deplacementsTour(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]);
int deplacementsFou(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]);
int deplacementsDame(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]);
int deplacementsRoi(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]);
int deplacementsCavalier(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]);
int deplacementsPion(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]);

int main() {
    const size_t temps_debut = time(NULL);
    char grille[N][N];
    int num_coup = 0;
    int couleurActuelle = C_BLANC;

    // Initialiser l' echiquier
    initialiseEchiquier(grille);

    // Boucle principale du jeu
    while (1) {
        effaceConsole();

        // Afficher l' echiquier
        afficheEchiquier(grille, num_coup, temps_debut, couleurActuelle);

        // Saisie et r ealisation du coup
        saisieCoup(grille, couleurActuelle);

        // V erifier fin de partie
        int etat = partieTerminee(grille, couleurAdverse(couleurActuelle));
        switch (etat) {
            case DEFAITE_BLANC:
                effaceConsole();
                if(TOURNER_ECHIQUIER) tournerEchiquier(grille);
                afficheEchiquier(grille, num_coup, temps_debut, couleurActuelle);
                ecranDeFin();
                printf("\n  \033[1;36mLe joueur noir gagne la partie!\033[0m\n\n");
                return 0;
            case DEFAITE_NOIR:
                effaceConsole();
                if(TOURNER_ECHIQUIER) tournerEchiquier(grille);
                afficheEchiquier(grille, num_coup, temps_debut, couleurActuelle);
                ecranDeFin();
                printf("\n  \033[1;36mLe joueur blanc gagne la partie!\033[0m\n\n");
                return 0;
            case PARTIE_NULLE:
                effaceConsole();
                if(TOURNER_ECHIQUIER) tournerEchiquier(grille);
                afficheEchiquier(grille, num_coup, temps_debut, couleurActuelle);
                ecranDeFin();
                printf("\n  \033[1;36mAucun des joueurs n'a pu s'imposer. Egalit e!\033[0m\n\n");
                return 0;
        }

        // Changer de joueur
        couleurActuelle = couleurAdverse(couleurActuelle);
        if(couleurActuelle == C_BLANC) {
            num_coup++;
        }
    }

    return 0;
}

void ecranDeFin() {
    printf(" ______   __     __   __        _____     ______        ______   ______     ______     ______   __     ______    \n");
    printf("/\\  ___\\ /\\ \\   /\\ \"-.\\ \\      /\\  __-.  /\\  ___\\      /\\  == \\ /\\  __ \\   /\\  == \\   /\\__  _\\ /\\ \\   /\\  ___\\   \n");
    printf("\\ \\  __\\ \\ \\ \\  \\ \\ \\-.  \\     \\ \\ \\/\\ \\ \\ \\  __\\      \\ \\  _-/ \\ \\  __ \\  \\ \\  __<   \\/_/\\ \\/ \\ \\ \\  \\ \\  __\\   \n");
    printf(" \\ \\_\\    \\ \\_\\  \\ \\_\\\"\\_ \\     \\ \\____-  \\ \\_____\\     \\ \\_\\    \\ \\_\\ \\_\\  \\ \\_\\ \\_\\    \\ \\_\\  \\ \\_\\  \\ \\_____\\ \n");
    printf("  \\/_/     \\/_/   \\/_/ \\/_/      \\/____/   \\/_____/      \\/_/     \\/_/\\/_/   \\/_/ /_/     \\/_/   \\/_/   \\/_____/ \n");
    printf("                                                                                                                  \n");
}

// INITIALISATION ET AFFICHAGE

bool estDansGrille(int ligne, int colonne) {
	return (ligne >= 0 && ligne < N) && (colonne >= 0 && colonne < N);
}

bool estCaseVide(const char grille[N][N], int ligne, int colonne) {
    return estDansGrille(ligne, colonne) && grille[ligne][colonne] == CASE_VIDE;
}

void videGrille(char grille[N][N]) {
    // Parcourt chaque ligne
    for (int i = 0; i < N; i++) {
        // Parcourt chaque colonne
        for (int j = 0; j < N; j++) {
            // Initialise la case à CASE_VIDE
            grille[i][j] = CASE_VIDE;
        }
    }
}

void copieGrille(char copie[N][N], const char grille[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            
            copie[i][j] = grille[i][j];
        }
    }
}

void initialiseEchiquier(char grille[N][N]){
        videGrille(grille);

        // Pièces noires
        grille[0][0] = grille[0][7] = 't';
        grille[0][1] = grille[0][6] = 'c';
        grille[0][2] = grille[0][5] = 'f';
        grille[0][4] = 'r';
        grille[0][3] = 'd';
        for (int i = 0; i < N; i++) {
            grille[1][i] = 'p';
        }

        // Pièces blanches
        grille[7][0] = grille[7][7] = 'T';
        grille[7][1] = grille[7][6] = 'C';
        grille[7][2] = grille[7][5] = 'F';
        grille[7][4] = 'R';
        grille[7][3] = 'D';
        for (int i = 0; i < N; i++) {
            grille[6][i] = 'P';
        }    
        
}

void afficheEchiquier(const char grille[N][N], int num_coup, const size_t temps_debut, int couleurActuelle) {
    printf(" ________                     ________      ___  ___      _______       ________       ________      \n");
    printf("|\\   ____\\                   |\\   ____\\    |\\  \\|\\  \\    |\\  ___ \\     |\\   ____\\     |\\   ____\\     \n");
    printf("\\ \\  \\___|     ____________  \\ \\  \\___|    \\ \\  \\\\\\  \\   \\ \\   __/|    \\ \\  \\___|_    \\ \\  \\___|_    \n");
    printf(" \\ \\  \\       |\\____________\\ \\ \\  \\        \\ \\   __  \\   \\ \\  \\_|/__   \\ \\_____  \\    \\ \\_____  \\   \n");
    printf("  \\ \\  \\____  \\|____________|  \\ \\  \\____    \\ \\  \\ \\  \\   \\ \\  \\_|\\ \\   \\|____|\\  \\    \\|____|\\  \\  \n");
    printf("   \\ \\_______\\                  \\ \\_______\\   \\ \\__\\ \\__\\   \\ \\_______\\    ____\\_\\  \\     ____\\_\\  \\ \n");
    printf("    \\|_______|                   \\|_______|    \\|__|\\|__|    \\|_______|   |\\_________\\   |\\_________\\\n");
    printf("                                                                          \\|_________|   \\|_________|\n");
    printf("                                                                                                      \n");
    printf("                                                                                                      \n");

    time_t temps_actuel;
    time(&temps_actuel);
    afficheTempsEcoule(temps_debut, temps_actuel);

    printf("  \033[1;36mTour: %d\033[0m\n", num_coup+1);
    printf("  \033[1;36mEn train de jouer:\033[0m \e[1;%dm%s\e[0m\n\n", couleurActuelle == C_BLANC ? 2+91 : 4+91, couleurActuelle == C_BLANC ? "Joueur Blanc" : "Joueur Noir");

    printf("   +---+---+---+---+---+---+---+---+\n");
    for (int ligne = 0; ligne < N; ligne++) {

        if(TOURNER_ECHIQUIER) {
            if(couleurActuelle == C_BLANC) {
                printf("%2d |", N - ligne);
            } else printf("%2d |", ligne + 1);
        } else {
            printf("%2d |", N - ligne);
        }

        for (int colonne = 0; colonne < N; colonne++) {
                printf(" ");
                if(estEnEchec(grille, C_BLANC)) {
                    afficheCaseEnCouleur(grille[ligne][colonne], C_BLANC);
                } else if(estEnEchec(grille, C_NOIR)) {
                    afficheCaseEnCouleur(grille[ligne][colonne], C_NOIR);
                } else {
                    afficheCaseEnCouleur(grille[ligne][colonne], C_VIDE);
                }

                printf(" |");
        }
        printf("\n   +---+---+---+---+---+---+---+---+\n");
    }
    if(TOURNER_ECHIQUIER) {
        if(couleurActuelle == C_BLANC) {
            printf("     A   B   C   D   E   F   G   H\n");
        } else {
            printf("     H   G   F   E   D   C   B   A\n");
        }
    } else printf("     A   B   C   D   E   F   G   H\n");

    printf("\n\n");
}

void afficheTempsEcoule(const size_t tempsDebut, time_t tempsActuel) {
    long tempsEcoule = difftime(tempsActuel, tempsDebut);

    int minutes = (int)tempsEcoule / 60; 
    int secondes = (int)tempsEcoule % 60;

    printf("  \033[1;36mTemps de la partie: %d minute(s) %d seconde(s)\033[0m\n", minutes, secondes);
}

void tournerEchiquier(char grille[N][N]) {
    if (TOURNER_ECHIQUIER) {
        char temp[N][N];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                temp[N - 1 - i][N - 1 - j] = grille[i][j];
            }
        }

        copieGrille(grille, temp);
    }
}

// GESTION DE COULEURS - SAISIE D'UN COUP

int couleurAdverse(int couleur) {
    if (couleur == C_BLANC) {
        return C_NOIR; 
    } else if (couleur == C_NOIR) {
        return C_BLANC; // Si la couleur est noire, retourne blanc
    }
        return -1;
}

int trouveCouleur(const char grille[N][N], int ligne, int colonne) {
    // V erifier si les coordonn ees sont hors de la grille
    if (!estDansGrille(ligne, colonne)) {
        return C_VIDE;
    }

    char piece = grille[ligne][colonne];

    // Case vide
    if ((estCaseVide(grille, ligne, colonne))) {
        return C_VIDE;
    }

    // D eterminer la couleur selon la casse
    if (estMajuscule(piece)) {
        return C_BLANC;
    } else {
        return C_NOIR;
    }
}

bool estMajuscule(char c) {
    return (c >= 'A' && c <= 'Z');
}

bool convertitEnCoordonnees(const char notation[2], int coordonnees[2]) {
    // Convertir la colonne (lettre)
    char colonne = notation[0];
    if (!estMajuscule(colonne)) {
        colonne = colonne - 'a' + 'A';
    }
    coordonnees[1] = colonne - 'A';

    // Convertir la ligne (chiffre)
    coordonnees[0] = '8' - notation[1];

    // V erifier si les coordonn ees sont dans la grille
    return estDansGrille(coordonnees[0], coordonnees[1]);
}

void saisieCoup(char grille[N][N], int couleurActuelle) {
    char notation[5];
    int depart[2], arrivee[2];
    bool coupValide = false;

    do {
        printf("  \033[1;36mEntrez votre coup (ex : E2E4) : \033[0m");
        scanf("%s", notation);

        if (strlen(notation) == 4 && convertitEnCoordonnees(notation, depart) && convertitEnCoordonnees(notation + 2, arrivee) && (trouveCouleur(grille, depart[0], depart[1]) == couleurActuelle)) {
            coupValide = realiseCoupSiValide(grille, depart, arrivee);
        }

        if (!coupValide) {
            printf("\033[1;31mLe coup saisi est invalide, retentez!\n\033[0m");
        }
    } while (!coupValide);

    printf("\033[0;32mLe coup est pris en compte.\033[0m\n");
    if(TOURNER_ECHIQUIER) tournerEchiquier(grille);
}

// JEU ECHEC

void trouvePositionRoi(const char grille[N][N], int positionRoi[2], int couleur) {
    char roi = (couleur == C_BLANC) ? 'R' : 'r';

    for (int ligne = 0; ligne < N; ligne++) {
        for (int colonne = 0; colonne < N; colonne++) {
            if (grille[ligne][colonne] == roi) {
                positionRoi[0] = ligne;
                positionRoi[1] = colonne;
                return;
            }
        }
    }

    // Roi non trouv e (ne devrait pas arriver dans un  etat de jeu valide)
    positionRoi[0] = -1;
    positionRoi[1] = -1;
}

void realiseCoup(char grille[N][N], const int depart[2], const int arrivee[2]) {
    // R ecup erer la pièce à d eplacer
    char piece = grille[depart[0]][depart[1]];

    // D eplacer la pièce vers la case d'arriv ee
    grille[arrivee[0]][arrivee[1]] = piece;

    // Vider la case de d epart
    grille[depart[0]][depart[1]] = ' ';

    // Promotion du pion
    int i = TOURNER_ECHIQUIER ? 0 : N-1;
    if ((piece == 'P' && arrivee[0] == 0) || (piece == 'p' && arrivee[0] == i)) {
        grille[arrivee[0]][arrivee[1]] = (piece == 'P') ? 'D' : 'd';
    }
}

bool realiseCoupSiValide(char grille[N][N], const int depart[2], const int arrivee[2]) {
    char grilleTemp[N][N];
    copieGrille(grilleTemp, grille);

    if (!estDeplacementValide(grilleTemp, depart, arrivee)) {
        return false;
    }

    int couleurJoueur = trouveCouleur(grilleTemp, depart[0], depart[1]);
    
    // R ealiser le coup
    realiseCoup(grilleTemp, depart, arrivee);

    // V erifier si notre propre roi n'est pas en  echec après le coup
    if (estEnEchec(grilleTemp, couleurJoueur)) {
        return false;
    }

    copieGrille(grille, grilleTemp);
    return true;
}

bool estEnEchec(const char grille[N][N], int couleur) {
    // Trouver la position du roi
    int positionRoi[2];
    trouvePositionRoi(grille, positionRoi, couleur);
    
    // Couleur adverse
    int adversaire = couleurAdverse(couleur);
    
    // Parcourir la grille pour trouver les pièces adverses
    for (int ligne = 0; ligne < N; ligne++) {
        for (int colonne = 0; colonne < N; colonne++) {
            if (trouveCouleur(grille, ligne, colonne) == adversaire) {
                int depart[2] = {ligne, colonne};
                // V erifier si la pièce peut capturer le roi SANS v erifier l' echec
                int deplacements[NB_MAX_DEPL][2];
                char piece = grille[ligne][colonne];
                int nbDeplacements;
                
                // Appeler directement la fonction de d eplacement appropri ee
                switch(piece) {
                    case 'T': case 't':
                        nbDeplacements = deplacementsTour(grille, depart, adversaire, deplacements);
                        break;
                    case 'F': case 'f':
                        nbDeplacements = deplacementsFou(grille, depart, adversaire, deplacements);
                        break;
                    case 'D': case 'd':
                        nbDeplacements = deplacementsDame(grille, depart, adversaire, deplacements);
                        break;
                    case 'C': case 'c':
                        nbDeplacements = deplacementsCavalier(grille, depart, adversaire, deplacements);
                        break;
                    case 'P': case 'p':
                        nbDeplacements = deplacementsPion(grille, depart, adversaire, deplacements);
                        break;
                    default:
                        continue;
                }
                
                // V erifier si un des d eplacements atteint le roi
                for (int i = 0; i < nbDeplacements; i++) {
                    if (deplacements[i][0] == positionRoi[0] && deplacements[i][1] == positionRoi[1]) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool estCoupValide(const char grille[N][N], const int depart[2], const int arrivee[2]) {
    // V erifier si la case de d epart est vide
    if (estCaseVide(grille, depart[0], depart[1])) {
        return false;
    }

    // Cr eer une copie de l' echiquier
    char grilleTemp[N][N];
    copieGrille(grilleTemp, grille);

    // R ealiser le coup
    realiseCoup(grilleTemp, depart, arrivee);

    // R ecup erer la couleur de la pièce d eplac ee
    int couleur = trouveCouleur(grille, depart[0], depart[1]);

    // V erifier si le roi de cette couleur est en  echec après le coup
    return !estEnEchec(grilleTemp, couleur);
}

bool estDeplacementValide(const char grille[N][N], const int depart[2], const int arrivee[2]) {
    int deplacementsValides[NB_MAX_DEPL][2];
    int nbDeplacements = listeDeplacementsValides(grille, depart, deplacementsValides);
    
    for (int i = 0; i < nbDeplacements; i++) {
        if (deplacementsValides[i][0] == arrivee[0] && deplacementsValides[i][1] == arrivee[1]) {
            // On ne v erifie pas l' echec ici, cela sera fait par realiseCoupSiValide
            return true;
        }
    }
    return false;
}

int listeDeplacementsValides(const char grille[N][N], const int depart[2], int deplacements[NB_MAX_DEPL][2]) {
    int nbDeplacements = 0;
    char piece = grille[depart[0]][depart[1]];
    
    // V erifier case vide
    if (piece == CASE_VIDE) return 0;
    
    int couleur = trouveCouleur(grille, depart[0], depart[1]);
    
    // D eplacements par type de pièce
    switch (piece) {
        case 'T': case 't':
            nbDeplacements = deplacementsTour(grille, depart, couleur, deplacements);
            break;
        case 'F': case 'f':
            nbDeplacements = deplacementsFou(grille, depart, couleur, deplacements);
            break;
        case 'D': case 'd':
            nbDeplacements = deplacementsDame(grille, depart, couleur, deplacements);
            break;
        case 'R': case 'r':
            nbDeplacements = deplacementsRoi(grille, depart, couleur, deplacements);
            break;
        case 'C': case 'c':
            nbDeplacements = deplacementsCavalier(grille, depart, couleur, deplacements);
            break;
        case 'P': case 'p':
            nbDeplacements = deplacementsPion(grille, depart, couleur, deplacements);
            break;
    }
    
    return nbDeplacements;
}

int deplacementsTour(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]) {
    int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    int nbDeplacements = 0;
    
    for (int d = 0; d < 4; d++) {
        int ligne = depart[0], colonne = depart[1];
        
        while (1) {
            ligne += directions[d][0];
            colonne += directions[d][1];
            
            if (!estDansGrille(ligne, colonne)) break;
            
            int couleurCase = trouveCouleur(grille, ligne, colonne);
            
            if (couleurCase == couleur) break;
            
            deplacements[nbDeplacements][0] = ligne;
            deplacements[nbDeplacements][1] = colonne;
            nbDeplacements++;
            
            if (couleurCase != C_VIDE) break;
        }
    }
    
    return nbDeplacements;
}

int deplacementsFou(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]) {
    int directions[4][2] = {{-1,-1}, {-1,1}, {1,-1}, {1,1}};
    int nbDeplacements = 0;
    
    for (int d = 0; d < 4; d++) {
        int ligne = depart[0], colonne = depart[1];
        
        while (1) {
            ligne += directions[d][0];
            colonne += directions[d][1];
            
            if (!estDansGrille(ligne, colonne)) break;
            
            int couleurCase = trouveCouleur(grille, ligne, colonne);
            
            if (couleurCase == couleur) break;
            
            deplacements[nbDeplacements][0] = ligne;
            deplacements[nbDeplacements][1] = colonne;
            nbDeplacements++;
            
            if (couleurCase != C_VIDE) break;
        }
    }
    
    return nbDeplacements;
}

int deplacementsDame(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]) {
    int directions[8][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    int nbDeplacements = 0;
    
    for (int d = 0; d < 8; d++) {
        int ligne = depart[0], colonne = depart[1];
        
        while (1) {
            ligne += directions[d][0];
            colonne += directions[d][1];
            
            if (!estDansGrille(ligne, colonne)) break;
            
            int couleurCase = trouveCouleur(grille, ligne, colonne);
            
            if (couleurCase == couleur) break;
            
            deplacements[nbDeplacements][0] = ligne;
            deplacements[nbDeplacements][1] = colonne;
            nbDeplacements++;
            
            if (couleurCase != C_VIDE) break;
        }
    }
    
    return nbDeplacements;
}

int deplacementsRoi(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]) {
    int directions[8][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    int nbDeplacements = 0;
    
    for (int d = 0; d < 8; d++) {
        int ligne = depart[0] + directions[d][0];
        int colonne = depart[1] + directions[d][1];
        
        if (estDansGrille(ligne, colonne)) {
            int couleurCase = trouveCouleur(grille, ligne, colonne);
            if (couleurCase != couleur) {
                // Cr eer une grille temporaire pour tester le coup
                char grilleTemp[N][N];
                copieGrille(grilleTemp, grille);
                
                // Simuler le d eplacement
                grilleTemp[ligne][colonne] = grille[depart[0]][depart[1]];
                grilleTemp[depart[0]][depart[1]] = CASE_VIDE;
                
                // V erifier si le roi n'est pas en  echec après le d eplacement
                if (!estEnEchec(grilleTemp, couleur)) {
                    deplacements[nbDeplacements][0] = ligne;
                    deplacements[nbDeplacements][1] = colonne;
                    nbDeplacements++;
                }
            }
        }
    }
    
    return nbDeplacements;
}

int deplacementsCavalier(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]) {
    int mouvements[8][2] = {{-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1}};
    int nbDeplacements = 0;
    
    for (int i = 0; i < 8; i++) {
        int ligne = depart[0] + mouvements[i][0];
        int colonne = depart[1] + mouvements[i][1];
        
        if (estDansGrille(ligne, colonne)) {
            int couleurCase = trouveCouleur(grille, ligne, colonne);
            if (couleurCase != couleur) {
                deplacements[nbDeplacements][0] = ligne;
                deplacements[nbDeplacements][1] = colonne;
                nbDeplacements++;
            }
        }
    }
    
    return nbDeplacements;
}

int deplacementsPion(const char grille[N][N], const int depart[2], int couleur, int deplacements[NB_MAX_DEPL][2]) {
    int nbDeplacements = 0;
    int direction;
    int ligneDepart;


    if(!TOURNER_ECHIQUIER) {
        direction = (couleur == C_BLANC) ? -1 : 1;
        ligneDepart = (couleur == C_BLANC) ? 6 : 1;
    } else {
        direction = -1;
        ligneDepart = 6;
    }

    // Avance simple
    int ligneAvance = depart[0] + direction;
    if (estDansGrille(ligneAvance, depart[1]) && trouveCouleur(grille, ligneAvance, depart[1]) == C_VIDE) {
        deplacements[nbDeplacements][0] = ligneAvance;
        deplacements[nbDeplacements][1] = depart[1];
        nbDeplacements++;

        // Double avance depuis la position initiale
        if (depart[0] == ligneDepart) {
            int ligneDoubleAvance = depart[0] + 2 * direction;
            if (estDansGrille(ligneDoubleAvance, depart[1]) && 
                trouveCouleur(grille, ligneDoubleAvance, depart[1]) == C_VIDE) {
                deplacements[nbDeplacements][0] = ligneDoubleAvance;
                deplacements[nbDeplacements][1] = depart[1];
                nbDeplacements++;
            }
        }
    }

    // Captures diagonales
    for (int delta = -1; delta <= 1; delta += 2) {
        int ligne = depart[0] + direction;
        int colonne = depart[1] + delta;
        if (estDansGrille(ligne, colonne)) {
            int couleurCase = trouveCouleur(grille, ligne, colonne);
            if (couleurCase != C_VIDE && couleurCase != couleur) {
                deplacements[nbDeplacements][0] = ligne;
                deplacements[nbDeplacements][1] = colonne;
                nbDeplacements++;
            }
        }
    }
    
    return nbDeplacements;
}

int partieTerminee(const char grille[N][N], int couleur) {
    // V erifier si le roi du joueur est en  echec
    if (estEnEchec(grille, couleur)) {
        // Parcourir toutes les pièces du joueur
        for (int ligne = 0; ligne < N; ligne++) {
            for (int colonne = 0; colonne < N; colonne++) {
                // Si la pièce appartient au joueur
                if (trouveCouleur(grille, ligne, colonne) == couleur) {
                    int depart[2] = {ligne, colonne};
                    // Obtenir tous les d eplacements possibles pour cette pièce
                    int deplacements[NB_MAX_DEPL][2];
                    int nbDeplacements = listeDeplacementsValides(grille, depart, deplacements);
                    
                    // Pour chaque d eplacement possible
                    for (int i = 0; i < nbDeplacements; i++) {
                        // Cr eer une copie temporaire de la grille
                        char grilleTemp[N][N];
                        copieGrille(grilleTemp, grille);
                        
                        // Tester le coup
                        realiseCoup(grilleTemp, depart, deplacements[i]);
                        
                        // Si ce coup sort de l' echec
                        if (!estEnEchec(grilleTemp, couleur)) {
                            return NON_TERMINEE; // Il existe au moins un coup valide
                        }
                    }
                }
            }
        }
        // Si aucun coup ne peut sortir de l' echec
        return (couleur == C_BLANC) ? DEFAITE_BLANC : DEFAITE_NOIR;
    }
    
    // Si le roi n'est pas en  echec, v erifier le pat
    for (int ligne = 0; ligne < N; ligne++) {
        for (int colonne = 0; colonne < N; colonne++) {
            if (trouveCouleur(grille, ligne, colonne) == couleur) {
                int depart[2] = {ligne, colonne};
                int deplacements[NB_MAX_DEPL][2];
                int nbDeplacements = listeDeplacementsValides(grille, depart, deplacements);
                
                // Pour chaque d eplacement possible
                for (int i = 0; i < nbDeplacements; i++) {
                    char grilleTemp[N][N];
                    copieGrille(grilleTemp, grille);
                    
                    // V erifier si le coup est l egal (ne met pas son propre roi en  echec)
                    realiseCoup(grilleTemp, depart, deplacements[i]);
                    if (!estEnEchec(grilleTemp, couleur)) {
                        return NON_TERMINEE;
                    }
                }
            }
        }
    }
    
    // Si aucun coup l egal n'est possible, c'est un pat
    return PARTIE_NULLE;
}

//////////////////////////////////////////
// Fonctions fournies:

void effaceConsole(void)
{
	LINUX ? system("clear") : system("cls");
}

// Si personne n'est en  echec, passer pour couleur C_VIDE.
void afficheCaseEnCouleur(char c, int couleurEnEchec)
{
	int couleurChar = estMajuscule(c) ? 2 : 4; // jaune ou mauve clair
	if ((c == 'R' && couleurEnEchec == C_BLANC) || (c == 'r' && couleurEnEchec == C_NOIR)) { // roi en  echec
		couleurChar = 0; // rouge
	}
	if (c == CASE_VIDE) {
		printf("%c", CASE_VIDE);
	}
	else {
		printf("\e[1;%dm%c\e[0m", couleurChar+91, c);
	}
}
