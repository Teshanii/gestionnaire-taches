#include "bouton.h"
#include <SDL/SDL_gfxPrimitives.h>
#include <string.h>

void creer_bouton(Bouton* b, int x, int y, int largeur, int hauteur, const char* texte) {
    
    if (b == NULL) {
        return;
    }

    b->x = x;
    b->y = y;
    b->largeur = largeur;
    b->hauteur = hauteur;
    strncpy(b->texte, texte, MAX_TEXTE_BOUTON - 1);
    b->texte[MAX_TEXTE_BOUTON - 1] = '\0';
    b->survole = 0;
    b->actif = 1;
}

void dessiner_bouton(SDL_Surface* ecran, Bouton* b) {
    
    if (b == NULL) || ecran == NULL) {
        return;
    }

    if (!b->actif) {
        return;
    }
    
    if (b->survole) {
        boxRGBA(ecran, b->x, b->y, b->x + b->largeur, b->y + b->hauteur, 
                41, 128, 185, 255);
    } else {
        boxRGBA(ecran, b->x, b->y, b->x + b->largeur, b->y + b->hauteur, 
                52, 152, 219, 255);
    }
    
    rectangleRGBA(ecran, b->x, b->y, b->x + b->largeur, b->y + b->hauteur, 
                  255, 255, 255, 255);
    
    int text_x = b->x + (b->largeur - (int)strlen(b->texte) * 8) / 2;
    int text_y = b->y + (b->hauteur - 8) / 2;
    
    stringRGBA(ecran, text_x, text_y, b->texte, 255, 255, 255, 255);
}

int bouton_survole(Bouton* b, int mouse_x, int mouse_y) {
    if (!b->actif) {
        return 0;
    }
    
    return (mouse_x >= b->x && mouse_x <= b->x + b->largeur &&
            mouse_y >= b->y && mouse_y <= b->y + b->hauteur);
}
