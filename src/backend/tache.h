#ifndef TACHE_H
#define TACHE_H

typedef struct{
    int id;
    char titre[100];
    char description[500];
    int est_terminee;

    int est_recurrente; // 0 = non, 1 = oui
    char type_recurrence[20]; // "quotidienne", "hebdomadaire", "mensuelle"

    //char date_creation[11]; // Format: YYYY-MM-DD
    //char date_echeance[11]; // Format: YYYY-MM-DD

    //int priorite; // 1 = basse, 2 = moyenne, 3 = haute
}Tache;

typedef struct{
    Tache taches[100];
    int nb_taches;
}GestionnaireTaches;

void init_gestionnaire(GestionnaireTaches* g); //initialier le gestionnaire avant de l'utiliser
void ajouter_tache(GestionnaireTaches* g, const char* titre, const char* description, int recurrente, const char* type_recurrence);
void afficher_taches(const GestionnaireTaches* g);
void marquer_terminee(GestionnaireTaches* g, int id);
void sauvegarder_taches(GestionnaireTaches* gt, const char* nom_fichier);
void charger_taches(GestionnaireTaches* gt, const char* nom_fichier);
void supprimer_tache(GestionnaireTaches* g, int id);

#endif

