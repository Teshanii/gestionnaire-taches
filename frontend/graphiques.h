#ifndef GRAPHIQUES_H
#define GRAPHIQUES_H

#include <SDL/SDL.h>
#include "../backend/tache.h"

void afficher_graphique_historique(SDL_Surface* ecran, GestionnaireTaches* g);
void afficher_graphique_priorites(SDL_Surface* ecran, GestionnaireTaches* g);
void afficher_graphique_temps_moyen(SDL_Surface* ecran, GestionnaireTaches* g);

#endif
