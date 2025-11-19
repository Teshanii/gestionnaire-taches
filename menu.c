#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusion du fichier d'en-tête  (tache.h)
#include "src/backend/tache.h"

/**
 * @brief Affiche le menu principal du gestionnaire de taches.
 */
void afficher_menu() {
    printf("\n========== MENU ==========\n");
    printf("1. Ajouter une tache\n");
    printf("2. Voir toutes les taches\n");
    printf("3. Marquer terminee\n");
    printf("4. Supprimer\n");
    printf("5. Quitter\n");
    printf("==========================\n");
    printf("Ton choix: ");
}

int main() {
    // 1. Créer et initialiser le gestionnaire de tâches (gt)
    GestionnaireTaches gt;
    init_gestionnaire(&gt);


    int choix;
    
    // 2. Boucle principale du menu (do-while)
    do {
        afficher_menu();

        // Récupérer le choix 
        if (scanf("%d", &choix) != 1) {
            // Gérer l'entrée non numérique
            while(getchar() != '\n'); 
            choix = 0; // Marquer le choix comme invalide
        }
        
        getchar(); // Consomme le caractère 'Enter' laissé par scanf()

        // 3. Traiter le choix 
        if (choix == 1) {
            // AJOUTER UNE TÂCHE
            char titre[100], desc[500];

            printf("Titre: ");
            fgets(titre, 100, stdin);
            titre[strcspn(titre, "\n")] = 0; // Enlève le caractère '\n'

            printf("Description: ");
            fgets(desc, 500, stdin);
            desc[strcspn(desc, "\n")] = 0; // Enlève le caractère '\n'
            
            // Appel de la fonction de Teshani
            ajouter_tache(&gt, titre, desc, 0, ""); // Correct: on passe l'adresse de gt
            printf("  Tâche ajoutee!\n");

        } else if (choix == 2) {
            // AFFICHER TOUTES LES TÂCHES
            // Appel de la fonction de Teshani
            afficher_taches(&gt); 

        } else if (choix == 3) {
            // MARQUER TERMINÉE
            int id;
            printf("ID de la tache: ");
            if (scanf("%d", &id) == 1) {
                // Appel de la fonction de Teshani
                marquer_terminee(&gt, id); // Correct: on passe l'adresse de gt
                printf("  Tache #%d marquée comme terminee.\n", id);
            } else {
                 printf("  ID invalide.\n");
            }
            getchar(); // Consomme le Enter

        } else if (choix == 4) {
            // SUPPRIMER
            int id;
            printf("ID de la tache: ");
            if (scanf("%d", &id) == 1) {
                // Appel de la fonction de Teshani
                supprimer_tache(&gt, id); // Correct: on passe l'adresse de gt
                printf("  Tache #%d supprimee (si elle existait).\n", id);
            } else {
                printf("  ID invalide.\n");
            }
            getchar(); // Consomme le Enter
            
        } else if (choix == 5) {
            // QUITTER
            printf("Au revoir! \n");

        } else {
            // Choix invalide
            printf("  Choix invalide! Veuillez choisir un numero entre 1 et 5.\n");
        }
    
    } while (choix != 5); // Continuer tant que le choix n'est pas 5

    return 0;
}