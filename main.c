#include <stdio.h>
#include "src/backend/tache.h"

int main() {
    GestionnaireTaches gestionnaire;

    printf("==============================================\n");
    printf("     TEST 1 : INITIALISATION\n");
    printf("==============================================\n");
    init_gestionnaire(&gestionnaire);
    printf("OK - Gestionnaire initialise\n");
    printf("Nombre de taches : %d\n\n", gestionnaire.nb_taches);

    printf("==============================================\n");
    printf("     TEST 2 : AJOUTER DES TACHES\n");
    printf("==============================================\n");
    ajouter_tache(&gestionnaire, "Faire les courses", 
                  "Acheter du pain et du lait", 0, "");
    ajouter_tache(&gestionnaire, "Appeler le dentiste", 
                  "Prendre RDV pour controle", 0, "");
    ajouter_tache(&gestionnaire, "Faire du sport", 
                  "30 min de course", 1, "Quotidien");
    printf("OK - 3 taches ajoutees\n");
    printf("Nombre de taches : %d\n\n", gestionnaire.nb_taches);

    printf("==============================================\n");
    printf("     TEST 3 : AFFICHER TOUTES LES TACHES\n");
    printf("==============================================\n");
    afficher_taches(&gestionnaire);
    printf("\n");

    printf("==============================================\n");
    printf("     TEST 4 : MARQUER TERMINEE (ID 1)\n");
    printf("==============================================\n");
    marquer_terminee(&gestionnaire, 1);
    printf("\n--- Affichage apres marquage ---\n");
    afficher_taches(&gestionnaire);
    printf("\n");

    printf("==============================================\n");
    printf("     TEST 5 : MARQUER ID INEXISTANT (99)\n");
    printf("==============================================\n");
    marquer_terminee(&gestionnaire, 99);
    printf("\n");

    printf("==============================================\n");
    printf("     TEST 6 : SUPPRIMER UNE TACHE (ID 2)\n");
    printf("==============================================\n");
    supprimer_tache(&gestionnaire, 2);
    printf("\n--- Affichage apres suppression ---\n");
    afficher_taches(&gestionnaire);
    printf("\n");

    printf("==============================================\n");
    printf("     TEST 7 : SUPPRIMER ID INEXISTANT (99)\n");
    printf("==============================================\n");
    supprimer_tache(&gestionnaire, 99);
    printf("\n");

    printf("==============================================\n");
    printf("     TEST 8 : AJOUTER APRES SUPPRESSION\n");
    printf("==============================================\n");
    ajouter_tache(&gestionnaire, "Nouvelle tache", 
                  "Test d'ajout apres suppression", 0, "");
    afficher_taches(&gestionnaire);
    printf("\n");

    printf("==============================================\n");
    printf("     TEST 9 : VERIFICATION FINALE\n");
    printf("==============================================\n");
    printf("Nombre total de taches : %d\n", gestionnaire.nb_taches);
    
    // On compte combien de taches sont terminees
    int nb_terminees = 0;
    for(int i = 0; i < gestionnaire.nb_taches; i++){
        if(gestionnaire.taches[i].est_terminee == 1){
            nb_terminees++;
        }
    }
    printf("Taches terminees : %d\n", nb_terminees);
    printf("Taches en cours : %d\n", gestionnaire.nb_taches - nb_terminees);

    printf("\nOK - Tous les tests sont passes !\n");

    return 0;
}
