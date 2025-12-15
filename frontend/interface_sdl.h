#ifndef INTERFACE_SDL_H
#define INTERFACE_SDL_H

#include <SDL/SDL.h>
#include "bouton.h"
#include "../backend/tache.h"

#define NB_BOUTONS_PRINCIPAUX 8
#define NB_BOUTONS_GRAPHIQUES 3

typedef struct {
    Bouton boutons_principaux[NB_BOUTONS_PRINCIPAUX];
    Bouton boutons_graphiques[NB_BOUTONS_GRAPHIQUES];
} InterfaceSDL;

void initialiser_interface(InterfaceSDL* interface, int largeur_ecran, int hauteur_ecran);
void afficher_interface(SDL_Surface* ecran, GestionnaireTaches* g, InterfaceSDL* interface, int mouse_x, int mouse_y);
int gerer_clic_bouton(InterfaceSDL* interface, int mouse_x, int mouse_y);

/* Fonctions de saisie */
int saisir_texte_sdl(SDL_Surface* ecran, char* buffer, int max_len, const char* titre);
int saisir_nombre_sdl(SDL_Surface* ecran, int min, int max, const char* titre);

/* Fonctions interactives */
void ajouter_tache_interactive(SDL_Surface* ecran, GestionnaireTaches* g);
void afficher_taches_sdl(SDL_Surface* ecran, GestionnaireTaches* g);
void marquer_terminee_interactive(SDL_Surface* ecran, GestionnaireTaches* g);
void supprimer_tache_interactive(SDL_Surface* ecran, GestionnaireTaches* g);
void modifier_tache_interactive(SDL_Surface* ecran, GestionnaireTaches* g);
void filtrer_par_priorite_interactive(SDL_Surface* ecran, GestionnaireTaches* g);
void afficher_message_temporaire(SDL_Surface* ecran, const char* msg, int duree);
void afficher_statistiques(SDL_Surface* ecran, GestionnaireTaches* g);

#endif
