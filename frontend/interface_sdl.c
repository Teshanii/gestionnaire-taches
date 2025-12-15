#include "interface_sdl.h"
#include "../backend/tache.h"
#include <SDL/SDL_gfxPrimitives.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Initialise tous les boutons de l'interface */
void initialiser_interface(InterfaceSDL* interface, int largeur_ecran, int hauteur_ecran) {
    int y_boutons = hauteur_ecran - 120;
    int espacement = 10;
    int largeur_bouton = (largeur_ecran - 90) / 4;
    int hauteur_bouton = 35;

    /* Première ligne */
    creer_bouton(&interface->boutons_principaux[0], 10, y_boutons, largeur_bouton, hauteur_bouton, "Ajouter");
    creer_bouton(&interface->boutons_principaux[1], 10 + (largeur_bouton + espacement), y_boutons, largeur_bouton, hauteur_bouton, "Supprimer");
    creer_bouton(&interface->boutons_principaux[2], 10 + 2*(largeur_bouton + espacement), y_boutons, largeur_bouton, hauteur_bouton, "Terminer");
    creer_bouton(&interface->boutons_principaux[3], 10 + 3*(largeur_bouton + espacement), y_boutons, largeur_bouton, hauteur_bouton, "Modifier");

    /* Deuxième ligne */
    y_boutons += hauteur_bouton + espacement;
    creer_bouton(&interface->boutons_principaux[4], 10, y_boutons, largeur_bouton, hauteur_bouton, "Tri Prio");
    creer_bouton(&interface->boutons_principaux[5], 10 + (largeur_bouton + espacement), y_boutons, largeur_bouton, hauteur_bouton, "Filtrer");
    creer_bouton(&interface->boutons_principaux[6], 10 + 2*(largeur_bouton + espacement), y_boutons, largeur_bouton, hauteur_bouton, "Stats");
    creer_bouton(&interface->boutons_principaux[7], 10 + 3*(largeur_bouton + espacement), y_boutons, largeur_bouton, hauteur_bouton, "Sauver");

    /* Boutons graphiques */
    int x_graph = largeur_ecran - 120;
    creer_bouton(&interface->boutons_graphiques[0], x_graph, 60, 110, 30, "Historique");
    creer_bouton(&interface->boutons_graphiques[1], x_graph, 95, 110, 30, "Priorites");
    creer_bouton(&interface->boutons_graphiques[2], x_graph, 130, 110, 30, "Temps Moy");
}

/* Gère les clics sur les boutons */
int gerer_clic_bouton(InterfaceSDL* interface, int mouse_x, int mouse_y) {
    int i;

    for (i = 0; i < NB_BOUTONS_PRINCIPAUX; i++) {
        if (bouton_survole(&interface->boutons_principaux[i], mouse_x, mouse_y)) {
            return i + 1;
        }
    }

    for (i = 0; i < NB_BOUTONS_GRAPHIQUES; i++) {
        if (bouton_survole(&interface->boutons_graphiques[i], mouse_x, mouse_y)) {
            return 9 + i;
        }
    }

    return 0;
}

/* Affiche l'interface complète */
void afficher_interface(SDL_Surface* ecran, GestionnaireTaches* g, InterfaceSDL* interface, int mouse_x, int mouse_y) {
    boxRGBA(ecran, 0, 0, ecran->w, ecran->h, 44, 62, 80, 255);

    stringRGBA(ecran, 20, 20, "=== GESTIONNAIRE DE TACHES ===", 255, 255, 255, 255);

    char total[50];
    snprintf(total, 50, "Total: %d taches", g->nb_taches);
    stringRGBA(ecran, 20, 50, total, 200, 200, 200, 255);

    int y = 90;
    Tache* courant = g->tete;

    while (courant != NULL && y < ecran->h - 180) {
        boxRGBA(ecran, 15, y - 5, ecran->w - 135, y + 55, 52, 73, 94, 255);

        char buffer[150];
        snprintf(buffer, 150, "[%d] %s", courant->id, courant->titre);
        stringRGBA(ecran, 25, y, buffer, 255, 255, 255, 255);

        snprintf(buffer, 150, "Priorite: %d | Echeance: %s", courant->priorite, courant->date_echeance);
        stringRGBA(ecran, 25, y + 20, buffer, 200, 200, 200, 255);

        if (courant->est_terminee) {
            stringRGBA(ecran, 25, y + 40, "[TERMINEE]", 46, 204, 113, 255);
        }

        y += 70;
        courant = courant->suivant;
    }

    int i;
    for (i = 0; i < NB_BOUTONS_PRINCIPAUX; i++) {
        interface->boutons_principaux[i].survole = bouton_survole(&interface->boutons_principaux[i], mouse_x, mouse_y);
        dessiner_bouton(ecran, &interface->boutons_principaux[i]);
    }

    for (i = 0; i < NB_BOUTONS_GRAPHIQUES; i++) {
        interface->boutons_graphiques[i].survole = bouton_survole(&interface->boutons_graphiques[i], mouse_x, mouse_y);
        dessiner_bouton(ecran, &interface->boutons_graphiques[i]);
    }

    SDL_Flip(ecran);
}

/* Saisie de texte avec SDL */
int saisir_texte_sdl(SDL_Surface* ecran, char* buffer, int max_len, const char* titre) {
    buffer[0] = '\0';
    int pos = 0;
    int termine = 0;
    int annule = 0;
    
    SDL_Event event;
    
    while (!termine && !annule) {
        boxRGBA(ecran, 0, 0, ecran->w, ecran->h, 44, 62, 80, 255);
        
        stringRGBA(ecran, 20, 50, titre, 255, 255, 255, 255);
        
        boxRGBA(ecran, 20, 100, ecran->w - 20, 140, 52, 73, 94, 255);
        stringRGBA(ecran, 30, 115, buffer, 255, 255, 255, 255);
        
        stringRGBA(ecran, 20, 200, "Appuyez sur ENTREE pour valider, ECHAP pour annuler", 200, 200, 200, 255);
        
        SDL_Flip(ecran);
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                annule = 1;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    termine = 1;
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    annule = 1;
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE && pos > 0) {
                    pos--;
                    buffer[pos] = '\0';
                }
                else if (pos < max_len - 1) {
                    char c = event.key.keysym.unicode;
                    if (c >= 32 && c < 127) {
                        buffer[pos++] = c;
                        buffer[pos] = '\0';
                    }
                }
            }
        }
        
        SDL_Delay(16);
    }
    
    return !annule && strlen(buffer) > 0;
}

/* Saisie d'un nombre */
int saisir_nombre_sdl(SDL_Surface* ecran, int min, int max, const char* titre) {
    char buffer[20];
    
    if (!saisir_texte_sdl(ecran, buffer, 20, titre)) {
        return -1;
    }
    
    int valeur = atoi(buffer);
    
    if (valeur < min) valeur = min;
    if (valeur > max) valeur = max;
    
    return valeur;
}

/* Ajoute une tâche de manière interactive */
void ajouter_tache_interactive(SDL_Surface* ecran, GestionnaireTaches* g) {
    char titre[MAX_TITRE];
    char desc[MAX_DESC];
    char echeance[11];
    
    if (!saisir_texte_sdl(ecran, titre, MAX_TITRE, "Entrez le titre de la tache:")) {
        return;
    }
    
    if (!saisir_texte_sdl(ecran, desc, MAX_DESC, "Entrez la description:")) {
        return;
    }
    
    int priorite = saisir_nombre_sdl(ecran, 1, 5, "Entrez la priorite (1-5):");
    if (priorite == -1) {
        return;
    }
    
    if (!saisir_texte_sdl(ecran, echeance, 11, "Entrez l'echeance (JJ/MM/AAAA):")) {
        return;
    }
    
    ajouter_tache(g, titre, desc, priorite, echeance);
    
    afficher_message_temporaire(ecran, "Tache ajoutee !", 1500);
}

/* Affiche toutes les tâches */
void afficher_taches_sdl(SDL_Surface* ecran, GestionnaireTaches* g) {
    boxRGBA(ecran, 0, 0, ecran->w, ecran->h, 44, 62, 80, 255);
    
    stringRGBA(ecran, 20, 20, "=== TOUTES LES TACHES ===", 255, 255, 255, 255);
    
    int y = 60;
    Tache* courant = g->tete;
    
    while (courant != NULL && y < ecran->h - 50) {
        char buffer[200];
        snprintf(buffer, 200, "[%d] %s - Prio: %d - %s", 
                courant->id, courant->titre, courant->priorite, 
                courant->est_terminee ? "TERMINEE" : "EN COURS");
        
        stringRGBA(ecran, 20, y, buffer, 255, 255, 255, 255);
        
        y += 30;
        courant = courant->suivant;
    }
    
    stringRGBA(ecran, 20, ecran->h - 30, "Appuyez sur une touche pour continuer...", 200, 200, 200, 255);
    
    SDL_Flip(ecran);
    
    SDL_Event event;
    int attente = 1;
    while (attente) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN || event.type == SDL_QUIT) {
                attente = 0;
            }
        }
        SDL_Delay(50);
    }
}

/* Marque une tâche comme terminée */
void marquer_terminee_interactive(SDL_Surface* ecran, GestionnaireTaches* g) {
    int id = saisir_nombre_sdl(ecran, 1, 9999, "Entrez l'ID de la tache a terminer:");
    
    if (id == -1) {
        return;
    }
    
    Tache* t = rechercher_tache(g, id);
    
    if (t == NULL) {
        afficher_message_temporaire(ecran, "Tache introuvable !", 1500);
        return;
    }
    
    if (t->est_terminee) {
        afficher_message_temporaire(ecran, "Tache deja terminee !", 1500);
        return;
    }
    
    marquer_terminee(g, id);
    afficher_message_temporaire(ecran, "Tache marquee comme terminee !", 1500);
}

/* Supprime une tâche */
void supprimer_tache_interactive(SDL_Surface* ecran, GestionnaireTaches* g) {
    int id = saisir_nombre_sdl(ecran, 1, 9999, "Entrez l'ID de la tache a supprimer:");
    
    if (id == -1) {
        return;
    }
    
    Tache* t = rechercher_tache(g, id);
    
    if (t == NULL) {
        afficher_message_temporaire(ecran, "Tache introuvable !", 1500);
        return;
    }
    
    supprimer_tache(g, id);
    afficher_message_temporaire(ecran, "Tache supprimee !", 1500);
}

/* Modifie une tâche */
void modifier_tache_interactive(SDL_Surface* ecran, GestionnaireTaches* g) {
    int id = saisir_nombre_sdl(ecran, 1, 9999, "Entrez l'ID de la tache a modifier:");
    
    if (id == -1) {
        return;
    }
    
    Tache* t = rechercher_tache(g, id);
    
    if (t == NULL) {
        afficher_message_temporaire(ecran, "Tache introuvable !", 1500);
        return;
    }
    
    char titre[MAX_TITRE];
    char desc[MAX_DESC];
    char echeance[11];
    
    if (!saisir_texte_sdl(ecran, titre, MAX_TITRE, "Nouveau titre (vide = garder):")) {
        return;
    }
    
    if (!saisir_texte_sdl(ecran, desc, MAX_DESC, "Nouvelle description (vide = garder):")) {
        return;
    }
    
    int priorite = saisir_nombre_sdl(ecran, 0, 5, "Nouvelle priorite (0 = garder):");
    
    if (!saisir_texte_sdl(ecran, echeance, 11, "Nouvelle echeance (vide = garder):")) {
        return;
    }
    
    modifier_tache(g, id, 
                   strlen(titre) > 0 ? titre : NULL,
                   strlen(desc) > 0 ? desc : NULL,
                   priorite,
                   strlen(echeance) > 0 ? echeance : NULL);
    
    afficher_message_temporaire(ecran, "Tache modifiee !", 1500);
}

/* Filtre par priorité */
void filtrer_par_priorite_interactive(SDL_Surface* ecran, GestionnaireTaches* g) {
    int priorite = saisir_nombre_sdl(ecran, 1, 5, "Entrez la priorite a filtrer (1-5):");
    
    if (priorite == -1) {
        return;
    }
    
    boxRGBA(ecran, 0, 0, ecran->w, ecran->h, 44, 62, 80, 255);
    
    char titre_buffer[100];
    snprintf(titre_buffer, 100, "=== TACHES DE PRIORITE %d ===", priorite);
    stringRGBA(ecran, 20, 20, titre_buffer, 255, 255, 255, 255);
    
    int y = 60;
    Tache* courant = g->tete;
    int trouve = 0;
    
    while (courant != NULL && y < ecran->h - 50) {
        if (courant->priorite == priorite) {
            char buffer[200];
            snprintf(buffer, 200, "[%d] %s - %s", 
                    courant->id, courant->titre, 
                    courant->est_terminee ? "TERMINEE" : "EN COURS");
            
            stringRGBA(ecran, 20, y, buffer, 255, 255, 255, 255);
            
            y += 30;
            trouve = 1;
        }
        courant = courant->suivant;
    }
    
    if (!trouve) {
        stringRGBA(ecran, 20, 60, "Aucune tache trouvee.", 200, 200, 200, 255);
    }
    
    stringRGBA(ecran, 20, ecran->h - 30, "Appuyez sur une touche pour continuer...", 200, 200, 200, 255);
    
    SDL_Flip(ecran);
    
    SDL_Event event;
    int attente = 1;
    while (attente) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN || event.type == SDL_QUIT) {
                attente = 0;
            }
        }
        SDL_Delay(50);
    }
}

/* Affiche un message temporaire */
void afficher_message_temporaire(SDL_Surface* ecran, const char* msg, int duree) {
    boxRGBA(ecran, 0, 0, ecran->w, ecran->h, 44, 62, 80, 255);
    
    int x = (ecran->w - (int)strlen(msg) * 8) / 2;
    int y = ecran->h / 2;
    
    stringRGBA(ecran, x, y, msg, 255, 255, 255, 255);
    
    SDL_Flip(ecran);
    SDL_Delay(duree);
}

/* Affiche les statistiques */
void afficher_statistiques(SDL_Surface* ecran, GestionnaireTaches* g) {
    boxRGBA(ecran, 0, 0, ecran->w, ecran->h, 44, 62, 80, 255);
    
    stringRGBA(ecran, 20, 20, "=== STATISTIQUES ===", 255, 255, 255, 255);
    
    int total = 0;
    int terminees = 0;
    int prio_count[6] = {0};
    
    Tache* courant = g->tete;
    while (courant != NULL) {
        total++;
        if (courant->est_terminee) {
            terminees++;
        }
        if (courant->priorite >= 1 && courant->priorite <= 5) {
            prio_count[courant->priorite]++;
        }
        courant = courant->suivant;
    }
    
    char buffer[200];
    int y = 60;
    
    snprintf(buffer, 200, "Nombre total de taches: %d", total);
    stringRGBA(ecran, 20, y, buffer, 255, 255, 255, 255);
    y += 30;
    
    snprintf(buffer, 200, "Taches terminees: %d", terminees);
    stringRGBA(ecran, 20, y, buffer, 46, 204, 113, 255);
    y += 30;
    
    snprintf(buffer, 200, "Taches en cours: %d", total - terminees);
    stringRGBA(ecran, 20, y, buffer, 231, 76, 60, 255);
    y += 30;
    
    if (total > 0) {
        snprintf(buffer, 200, "Taux de completion: %.1f%%", (terminees * 100.0) / total);
        stringRGBA(ecran, 20, y, buffer, 52, 152, 219, 255);
        y += 50;
    }
    
    stringRGBA(ecran, 20, y, "Repartition par priorite:", 255, 255, 255, 255);
    y += 30;
    
    int i;
    for (i = 1; i <= 5; i++) {
        snprintf(buffer, 200, "  Priorite %d: %d taches", i, prio_count[i]);
        stringRGBA(ecran, 40, y, buffer, 200, 200, 200, 255);
        y += 25;
    }
    
    stringRGBA(ecran, 20, ecran->h - 30, "Appuyez sur une touche pour continuer...", 200, 200, 200, 255);
    
    SDL_Flip(ecran);
    
    SDL_Event event;
    int attente = 1;
    while (attente) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN || event.type == SDL_QUIT) {
                attente = 0;
            }
        }
        SDL_Delay(50);
    }
}
