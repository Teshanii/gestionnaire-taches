#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tache.h"

#define FICHIER "data/taches.txt"

void afficher_menu() {
    printf("\n===== GESTIONNAIRE DE TACHES =====\n");
    printf("1. Ajouter une tache\n");
    printf("2. Afficher toutes les taches\n");
    printf("3. Marquer une tache comme terminee\n");
    printf("4. Supprimer une tache\n");
    printf("5. Quitter\n");
    printf("==================================\n");
    printf("Votre choix: ");
}

int main() {
    GestionnaireTaches gestionnaire;
    init_gestionnaire(&gestionnaire);

    charger_taches(&gestionnaire, FICHIER);

    int choix;
    char buffer[100]; // pour lire l'entrée

    do {
        afficher_menu();

        // lire l'entrée comme une chaine
        if (fgets(buffer, 100, stdin) == NULL) {
            printf("\nErreur de lecture!\n");
            continue;
        }

        // essayer de convertir en nombre
        if (sscanf(buffer, "%d", &choix) != 1) {
            printf("\n>>> ERREUR : Veuillez entrer un numero entre 1 et 5 !\n");
            continue;
        }

        if (choix == 1) {
            // ajouter une tache
            char titre[100];
            char description[500];
            int est_recurrente;
            char type_recurrence[20] = "";

            printf("\nTitre de la tache: ");
            fgets(titre, 100, stdin);
            titre[strlen(titre)-1] = '\0';

            printf("Description: ");
            fgets(description, 500, stdin);
            description[strlen(description)-1] = '\0';

            printf("Tache recurrente? (0=non, 1=oui): ");
            fgets(buffer, 100, stdin);
            sscanf(buffer, "%d", &est_recurrente);

            if (est_recurrente == 1) {
                printf("\nType de recurrence:\n");
                printf("  1. Quotidienne\n");
                printf("  2. Hebdomadaire\n");
                printf("  3. Mensuelle\n");
                printf("Votre choix: ");

                int type;
                fgets(buffer, 100, stdin);
                sscanf(buffer, "%d", &type);

                if (type == 1) {
                    strcpy(type_recurrence, "Quotidienne");
                } else if (type == 2) {
                    strcpy(type_recurrence, "Hebdomadaire");
                } else if (type == 3) {
                    strcpy(type_recurrence, "Mensuelle");
                } else {
                    strcpy(type_recurrence, "Quotidienne");
                }
            }

            ajouter_tache(&gestionnaire, titre, description, est_recurrente, type_recurrence);
            printf("\nTache ajoutee avec succes!\n");

            sauvegarder_taches(&gestionnaire, FICHIER);

        } else if (choix == 2) {
            printf("\n");
            afficher_taches(&gestionnaire);

        } else if (choix == 3) {
            if (gestionnaire.nb_taches == 0) {
                printf("\nAucune tache a marquer!\n");
                continue;
            }

            int id;
            printf("\nID de la tache a marquer: ");
            fgets(buffer, 100, stdin);
            sscanf(buffer, "%d", &id);

            marquer_terminee(&gestionnaire, id);
            sauvegarder_taches(&gestionnaire, FICHIER);

        } else if (choix == 4) {
            if (gestionnaire.nb_taches == 0) {
                printf("\nAucune tache a supprimer!\n");
                continue;
            }

            int id;
            printf("\nID de la tache a supprimer: ");
            fgets(buffer, 100, stdin);
            sscanf(buffer, "%d", &id);

            supprimer_tache(&gestionnaire, id);
            sauvegarder_taches(&gestionnaire, FICHIER);

        } else if (choix == 5) {
            sauvegarder_taches(&gestionnaire, FICHIER);
            printf("\nA bientot!\n");

        } else {
            printf("\n>>> ERREUR : Choix invalide! Choisissez entre 1 et 5.\n");
        }

    } while (choix != 5);

    return 0;
}
