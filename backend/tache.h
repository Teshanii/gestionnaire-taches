#ifndef TACHE_H
#define TACHE_H

#define MAX_TITRE 100
#define MAX_DESC 500
#define MAX_HISTORIQUE 30

/* Structure d'une tâche (liste chaînée) */
typedef struct Tache {
    int id;
    char titre[MAX_TITRE];
    char description[MAX_DESC];
    int priorite;
    char date_echeance[11];
    int est_terminee;
    int nb_completions;
    char date_creation[11];
    char date_completion[11];
    int jours_pour_terminer;
    struct Tache* suivant;
} Tache;

/* Structure du gestionnaire */
typedef struct {
    Tache* tete;
    int nb_taches;
    int prochain_id;
    int historique_completions[MAX_HISTORIQUE];
    char dates_historique[MAX_HISTORIQUE][11];
    int nb_historique;
} GestionnaireTaches;

/* Fonctions de gestion */
void initialiser_gestionnaire(GestionnaireTaches* g);
void ajouter_tache(GestionnaireTaches* g, const char* titre, const char* desc, 
                   int priorite, const char* echeance);
void supprimer_tache(GestionnaireTaches* g, int id);
void modifier_tache(GestionnaireTaches* g, int id, const char* nouveau_titre, 
                    const char* nouvelle_desc, int nouvelle_priorite, 
                    const char* nouvelle_echeance);
void marquer_terminee(GestionnaireTaches* g, int id);
Tache* rechercher_tache(GestionnaireTaches* g, int id);

/* Fonctions de persistance */
void sauvegarder_taches(const GestionnaireTaches* g, const char* fichier);
void charger_taches(GestionnaireTaches* g, const char* fichier);
void liberer_gestionnaire(GestionnaireTaches* g);

/* Fonction pour mettre à jour l'historique */
void ajouter_completion_historique(GestionnaireTaches* g, const char* date);

#endif
