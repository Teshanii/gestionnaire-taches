#include "graphiques.h"
#include <SDL/SDL_gfxPrimitives.h>
#include <stdio.h>
#include <string.h>

/* Affiche l'historique des complétions */
void afficher_graphique_historique(SDL_Surface* ecran, GestionnaireTaches* g) {
    
    if (g == NULL) || ecran == NULL) {
        return;
    }
    
    boxRGBA(ecran, 0, 0, ecran->w, ecran->h, 44, 62, 80, 255);
    
    stringRGBA(ecran, 20, 20, "=== HISTORIQUE DES COMPLETIONS ===", 255, 255, 255, 255);
    
    if (g->nb_historique == 0) {
        stringRGBA(ecran, 20, 60, "Aucune donnee disponible.", 200, 200, 200, 255);
    } else {
        int max_val = 0;
        int i;
        
        for (i = 0; i < g->nb_historique; i++) {
            if (g->historique_completions[i] > max_val) {
                max_val = g->historique_completions[i];
            }
        }
        
        if (max_val == 0) max_val = 1;
        
        int largeur_barre = (ecran->w - 100) / (g->nb_historique > 10 ? 10 : g->nb_historique);
        int hauteur_max = ecran->h - 150;
        
        for (i = 0; i < g->nb_historique && i < 10; i++) {
            int hauteur = (g->historique_completions[i] * hauteur_max) / max_val;
            
            int x = 50 + i * (largeur_barre + 10);
            int y_base = ecran->h - 80;
            
            boxRGBA(ecran, x, y_base - hauteur, x + largeur_barre, y_base, 
                    52, 152, 219, 255);
            
            rectangleRGBA(ecran, x, y_base - hauteur, x + largeur_barre, y_base, 
                          255, 255, 255, 255);
            
            char val_str[10];
            snprintf(val_str, 10, "%d", g->historique_completions[i]);
            stringRGBA(ecran, x + 5, y_base - hauteur - 20, val_str, 255, 255, 255, 255);
            
            char date_short[6];
            strncpy(date_short, g->dates_historique[i], 5);
            date_short[5] = '\0';
            stringRGBA(ecran, x, y_base + 5, date_short, 200, 200, 200, 255);
        }
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

/* Affiche la répartition des priorités */
void afficher_graphique_priorites(SDL_Surface* ecran, GestionnaireTaches* g) {
    boxRGBA(ecran, 0, 0, ecran->w, ecran->h, 44, 62, 80, 255);
    
    stringRGBA(ecran, 20, 20, "=== REPARTITION PAR PRIORITE ===", 255, 255, 255, 255);
    
    int prio_count[6] = {0};
    int total = 0;
    
    Tache* courant = g->tete;
    while (courant != NULL) {
        if (courant->priorite >= 1 && courant->priorite <= 5) {
            prio_count[courant->priorite]++;
            total++;
        }
        courant = courant->suivant;
    }
    
    if (total == 0) {
        stringRGBA(ecran, 20, 60, "Aucune tache disponible.", 200, 200, 200, 255);
    } else {
        int max_count = 0;
        int i;
        
        for (i = 1; i <= 5; i++) {
            if (prio_count[i] > max_count) {
                max_count = prio_count[i];
            }
        }
        
        if (max_count == 0) max_count = 1;
        
        int largeur_max = ecran->w - 250;
        int y_base = 80;
        
        for (i = 1; i <= 5; i++) {
            int largeur = (prio_count[i] * largeur_max) / max_count;
            int y = y_base + (i - 1) * 70;
            
            char label[50];
            snprintf(label, 50, "Priorite %d:", i);
            stringRGBA(ecran, 20, y + 15, label, 255, 255, 255, 255);
            
            if (prio_count[i] > 0) {
                boxRGBA(ecran, 150, y, 150 + largeur, y + 40, 
                        52, 152, 219, 255);
                
                rectangleRGBA(ecran, 150, y, 150 + largeur, y + 40, 
                              255, 255, 255, 255);
                
                char val_str[20];
                sprintf(val_str, "%d (%.1f%%)", prio_count[i], (prio_count[i] * 100.0) / total);
                stringRGBA(ecran, 160 + largeur, y + 15, val_str, 255, 255, 255, 255);
            } else {
                stringRGBA(ecran, 150, y + 15, "0", 200, 200, 200, 255);
            }
        }
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

/* Affiche le temps moyen de complétion */
void afficher_graphique_temps_moyen(SDL_Surface* ecran, GestionnaireTaches* g) {
    boxRGBA(ecran, 0, 0, ecran->w, ecran->h, 44, 62, 80, 255);
    
    stringRGBA(ecran, 20, 20, "=== TEMPS MOYEN DE COMPLETION ===", 255, 255, 255, 255);
    
    int prio_count[6] = {0};
    int prio_jours[6] = {0};
    
    Tache* courant = g->tete;
    while (courant != NULL) {
        if (courant->est_terminee && courant->priorite >= 1 && courant->priorite <= 5) {
            prio_count[courant->priorite]++;
            prio_jours[courant->priorite] += courant->jours_pour_terminer;
        }
        courant = courant->suivant;
    }
    
    int total_terminees = 0;
    int i;
    for (i = 1; i <= 5; i++) {
        total_terminees += prio_count[i];
    }
    
    if (total_terminees == 0) {
        stringRGBA(ecran, 20, 60, "Aucune tache terminee.", 200, 200, 200, 255);
    } else {
        int y_base = 80;
        
        for (i = 1; i <= 5; i++) {
            int y = y_base + (i - 1) * 60;
            
            char label[50];
            snprintf(label, 50, "Priorite %d:", i);
            stringRGBA(ecran, 20, y + 10, label, 255, 255, 255, 255);
            
            if (prio_count[i] > 0) {
                double moyenne = (double)prio_jours[i] / prio_count[i];
                
                char val_str[50];
                snprintf(val_str, 50, "%.1f jours (sur %d taches)", moyenne, prio_count[i]);
                stringRGBA(ecran, 150, y + 10, val_str, 46, 204, 113, 255);
            } else {
                stringRGBA(ecran, 150, y + 10, "Aucune donnee", 200, 200, 200, 255);
            }
        }
        
        double moyenne_globale = 0;
        int total_jours = 0;
        for (i = 1; i <= 5; i++) {
            total_jours += prio_jours[i];
        }
        
        if (total_terminees > 0) {
            moyenne_globale = (double)total_jours / total_terminees;
            
            char global_str[100];
            snprintf(global_str, 100, "Moyenne globale: %.1f jours", moyenne_globale);
            stringRGBA(ecran, 20, y_base + 350, global_str, 255, 255, 0, 255);
        }
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
