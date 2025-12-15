#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "tache.h"
#include "bouton.h"
#include "interface_sdl.h"
#include "graphiques.h"

#define LARGEUR_ECRAN 800
#define HAUTEUR_ECRAN 600

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur SDL : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Surface* ecran = SDL_SetVideoMode(LARGEUR_ECRAN, HAUTEUR_ECRAN, 32, 
                                           SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (ecran == NULL) {
        fprintf(stderr, "Erreur creation fenetre : %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_WM_SetCaption("Gestionnaire de Taches - ESGI", NULL);
    SDL_EnableUNICODE(1);

    GestionnaireTaches gestionnaire;
    initialiser_gestionnaire(&gestionnaire);
    charger_taches(&gestionnaire, "taches.txt");

    InterfaceSDL interface;
    initialiser_interface(&interface, LARGEUR_ECRAN, HAUTEUR_ECRAN);

    int mouse_x = 0, mouse_y = 0;
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_MOUSEMOTION) {
                mouse_x = event.motion.x;
                mouse_y = event.motion.y;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int action = gerer_clic_bouton(&interface, mouse_x, mouse_y);

                switch (action) {
                    case 1:
                        ajouter_tache_interactive(ecran, &gestionnaire);
                        break;

                    case 2:
                        supprimer_tache_interactive(ecran, &gestionnaire);
                        break;

                    case 3:
                        marquer_terminee_interactive(ecran, &gestionnaire);
                        break;

                    case 4:
                        modifier_tache_interactive(ecran, &gestionnaire);
                        break;

                    case 5:
                        afficher_taches_sdl(ecran, &gestionnaire);
                        break;

                    case 6:
                        filtrer_par_priorite_interactive(ecran, &gestionnaire);
                        break;

                    case 7:
                        afficher_statistiques(ecran, &gestionnaire);
                        break;

                    case 8:
                        sauvegarder_taches(&gestionnaire, "taches.txt");
                        afficher_message_temporaire(ecran, "Taches sauvegardees !", 1500);
                        break;

                    case 9:
                        afficher_graphique_historique(ecran, &gestionnaire);
                        break;

                    case 10:
                        afficher_graphique_priorites(ecran, &gestionnaire);
                        break;

                    case 11:
                        afficher_graphique_temps_moyen(ecran, &gestionnaire);
                        break;

                    default:
                        break;
                }
            }
        }

        afficher_interface(ecran, &gestionnaire, &interface, mouse_x, mouse_y);

        SDL_Delay(16);
    }

    sauvegarder_taches(&gestionnaire, "taches.txt");
    liberer_gestionnaire(&gestionnaire);

    SDL_Quit();

    return EXIT_SUCCESS;
}
