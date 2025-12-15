#include "tache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Initialise le gestionnaire */
void initialiser_gestionnaire(GestionnaireTaches* g) {
    
    if(g == NULL) {
        return;
    }

    g->tete = NULL;
    g->nb_taches = 0;
    g->prochain_id = 1;
    g->nb_historique = 0;
    
    int i;
    for (i = 0; i < MAX_HISTORIQUE; i++) {
        g->historique_completions[i] = 0;
        g->dates_historique[i][0] = '\0';
    }
}

/* Recherche une tâche par ID */
Tache* rechercher_tache(GestionnaireTaches* g, int id) {
    
    if (g == NULL) {
        return NULL;
    }

    Tache* courant = g->tete;
    
    while (courant != NULL) {
        if (courant->id == id) {
            return courant;
        }
        courant = courant->suivant;
    }
    
    return NULL;
}

/* Ajoute une tâche */
void ajouter_tache(GestionnaireTaches* g, const char* titre, const char* desc, 
                   int priorite, const char* echeance) {
    Tache* nouvelle = (Tache*)malloc(sizeof(Tache));
    
    if (nouvelle == NULL) {
        fprintf(stderr, "Erreur allocation memoire\n");
        return;
    }
    
    nouvelle->id = g->prochain_id++;
    strncpy(nouvelle->titre, titre, MAX_TITRE - 1);
    nouvelle->titre[MAX_TITRE - 1] = '\0';
    
    strncpy(nouvelle->description, desc, MAX_DESC - 1);
    nouvelle->description[MAX_DESC - 1] = '\0';
    
    nouvelle->priorite = priorite;
    
    strncpy(nouvelle->date_echeance, echeance, 10);
    nouvelle->date_echeance[10] = '\0';
    
    nouvelle->est_terminee = 0;
    nouvelle->nb_completions = 0;
    nouvelle->jours_pour_terminer = 0;
    
    /* Date de création */
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(nouvelle->date_creation, sizeof(nouvelle->date_creation), "%02d/%02d/%04d", 
        t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);

    
    nouvelle->date_completion[0] = '\0';
    
    /* Insertion en tête */
    nouvelle->suivant = g->tete;
    g->tete = nouvelle;
    g->nb_taches++;
}

/* Supprime une tâche */
void supprimer_tache(GestionnaireTaches* g, int id) {
    
    if (g == NULL) {
        return;
    }

    Tache* courant = g->tete;
    Tache* precedent = NULL;
    
    while (courant != NULL) {
        if (courant->id == id) {
            if (precedent == NULL) {
                g->tete = courant->suivant;
            } else {
                precedent->suivant = courant->suivant;
            }
            
            free(courant);
            g->nb_taches--;
            return;
        }
        
        precedent = courant;
        courant = courant->suivant;
    }
}

/* Modifie une tâche */
void modifier_tache(GestionnaireTaches* g, int id, const char* nouveau_titre, 
                    const char* nouvelle_desc, int nouvelle_priorite, 
                    const char* nouvelle_echeance) {
    Tache* t = rechercher_tache(g, id);
    
    if (t == NULL) {
        return;
    }
    
    if (nouveau_titre != NULL) {
        strncpy(t->titre, nouveau_titre, MAX_TITRE - 1);
        t->titre[MAX_TITRE - 1] = '\0';
    }
    
    if (nouvelle_desc != NULL) {
        strncpy(t->description, nouvelle_desc, MAX_DESC - 1);
        t->description[MAX_DESC - 1] = '\0';
    }
    
    if (nouvelle_priorite > 0) {
        t->priorite = nouvelle_priorite;
    }
    
    if (nouvelle_echeance != NULL) {
        strncpy(t->date_echeance, nouvelle_echeance, 10);
        t->date_echeance[10] = '\0';
    }
}

/* Marque une tâche comme terminée */
void marquer_terminee(GestionnaireTaches* g, int id) {
    Tache* t = rechercher_tache(g, id);
    
    if (t == NULL || t->est_terminee) {
        return;
    }
    
    t->est_terminee = 1;
    t->nb_completions++;
    
    /* Date de complétion */
    time_t now = time(NULL);
    struct tm* t_local = localtime(&now);
    snprintf(t->date_completion, sizeof(t->date_completion), "%02d/%02d/%04d", 
        t_local->tm_mday, t_local->tm_mon + 1, t_local->tm_year + 1900);

    
    /* Calcul simplifié de la durée (en jours) */
    t->jours_pour_terminer = 1;
    
    /* Ajoute à l'historique */
    ajouter_completion_historique(g, t->date_completion);
}

/* Sauvegarde les tâches dans un fichier */
void sauvegarder_taches(const GestionnaireTaches* g, const char* fichier) {
    FILE* f = fopen(fichier, "w");
    
    if (f == NULL) {
        fprintf(stderr, "Erreur ouverture fichier %s\n", fichier);
        return;
    }
    
    Tache* courant = g->tete;
    
    while (courant != NULL) {
        fprintf(f, "%d|%s|%s|%d|%s|%d|%d|%s|%s|%d\n",
                courant->id,
                courant->titre,
                courant->description,
                courant->priorite,
                courant->date_echeance,
                courant->est_terminee,
                courant->nb_completions,
                courant->date_creation,
                courant->date_completion,
                courant->jours_pour_terminer);
        
        courant = courant->suivant;
    }
    
    fclose(f);
}

/* Charge les tâches depuis un fichier */
void charger_taches(GestionnaireTaches* g, const char* fichier) {
    FILE* f = fopen(fichier, "r");
    
    if (f == NULL) {
        return;
    }
    
    char ligne[1024];
    
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        Tache* nouvelle = (Tache*)malloc(sizeof(Tache));
        
        if (nouvelle == NULL) {
            continue;
        }
        
        sscanf(ligne, "%d|%[^|]|%[^|]|%d|%[^|]|%d|%d|%[^|]|%[^|]|%d",
               &nouvelle->id,
               nouvelle->titre,
               nouvelle->description,
               &nouvelle->priorite,
               nouvelle->date_echeance,
               &nouvelle->est_terminee,
               &nouvelle->nb_completions,
               nouvelle->date_creation,
               nouvelle->date_completion,
               &nouvelle->jours_pour_terminer);
        
        nouvelle->suivant = g->tete;
        g->tete = nouvelle;
        g->nb_taches++;
        
        if (nouvelle->id >= g->prochain_id) {
            g->prochain_id = nouvelle->id + 1;
        }
    }
    
    fclose(f);
}

/* Libère la mémoire */
void liberer_gestionnaire(GestionnaireTaches* g) {
    Tache* courant = g->tete;
    
    while (courant != NULL) {
        Tache* suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    
    g->tete = NULL;
    g->nb_taches = 0;
}

/* Ajoute une complétion à l'historique */
void ajouter_completion_historique(GestionnaireTaches* g, const char* date) {
    int i;
    
    /* Vérifie si la date existe déjà */
    for (i = 0; i < g->nb_historique; i++) {
        if (strcmp(g->dates_historique[i], date) == 0) {
            g->historique_completions[i]++;
            return;
        }
    }
    
    /* Ajoute une nouvelle date */
    if (g->nb_historique < MAX_HISTORIQUE) {
        strcpy(g->dates_historique[g->nb_historique], date);
        g->historique_completions[g->nb_historique] = 1;
        g->nb_historique++;
    } else {
        /* Décale tout vers la gauche */
        for (i = 0; i < MAX_HISTORIQUE - 1; i++) {
            g->historique_completions[i] = g->historique_completions[i + 1];
            strcpy(g->dates_historique[i], g->dates_historique[i + 1]);
        }
        
        strcpy(g->dates_historique[MAX_HISTORIQUE - 1], date);
        g->historique_completions[MAX_HISTORIQUE - 1] = 1;
    }
}
