#ifndef BOUTON_H
#define BOUTON_H

#include <SDL/SDL.h>

#define MAX_TEXTE_BOUTON 50

typedef struct {
    int x, y;
    int largeur, hauteur;
    char texte[MAX_TEXTE_BOUTON];
    int survole;
    int actif;
} Bouton;

void creer_bouton(Bouton* b, int x, int y, int largeur, int hauteur, const char* texte);
void dessiner_bouton(SDL_Surface* ecran, Bouton* b);
int bouton_survole(Bouton* b, int mouse_x, int mouse_y);

#endif
