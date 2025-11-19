#include "tache.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void init_gestionnaire(GestionnaireTaches* g) {
    g->nb_taches = 0; //meme chose que (*g).nb_taches = 0;
    //va a l'adresse stockeer dans g 
    //après on accède au champ nb_taches pour le mettre a 0
}

void ajouter_tache(GestionnaireTaches* g, const char* titre, const char* description, int recurrente, const char* type_recurrence){
    if (g->nb_taches >=100){
        printf("Nombre maximum de taches atteint.\n");
        return;
    }

    if (g->nb_taches == 0){
        g->taches[0].id = 1;
    } else{
        //trouver l'index de la derniere tache
        int index_last = g->nb_taches -1;

        //recuperer son id
        int id_last = g->taches[index_last].id;

        //calcul du nouvel id
        int new_id = id_last + 1;

        g->taches[g->nb_taches].id = new_id;
    }

    strncpy(g->taches[g->nb_taches].titre, titre,99); //copier le titre
    //si titre fait plus de 99 char il y aura un debordement de memoire
    g->taches[g->nb_taches].titre[99] = '\0'; //ajouter le caractere de fin de chaine

    strncpy(g->taches[g->nb_taches].description, description,499);
    g->taches[g->nb_taches].description[499] = '\0';

    g->taches[g->nb_taches].est_terminee = 0; //comme on vient d'ajouter il faut mettre a 0

    g->taches[g->nb_taches].est_recurrente = recurrente;

    strcpy(g->taches[g->nb_taches].type_recurrence, type_recurrence);

    g->nb_taches++; //incrementer car sinon on ecrase la tache precedente

}

void afficher_taches(const GestionnaireTaches* g){ //const permet de dire qu'on va juste lire et pas modifier
    if (g->nb_taches == 0){
        printf("Aucune tache a afficher.\n");
        return;
    } else{
        for(int i = 0; i < g->nb_taches; i++){
            printf("ID: %d\n", g->taches[i].id);
            
            printf("Titre: %s\n", g->taches[i].titre);
            
            printf("Description: %s\n", g->taches[i].description);
            
            if (g->taches[i].est_terminee == 1) {
                printf("Statut: Terminee\n");
            } else {
                printf("Statut: En cours\n");
            }
            
            if (g->taches[i].est_recurrente == 1){ //pareille que de mettre sans les == 1
                printf("Recurrente: Oui (%s)\n", g->taches[i].type_recurrence);
            } else{
                printf("Recurrente: Non\n");
            }
            
            printf("\n");
        }
    }
}

void marquer_terminee(GestionnaireTaches*g, int id){
    for(int i =0; i < g->nb_taches; i++){
        if(g->taches[i].id == id){
            g->taches[i].est_terminee = 1;
            printf("Tache '%s' marquee comme terminee. \n", g->taches[i].titre);
            return;
        }
    }
    printf("Erreur: aucune tache avec l'ID %d a ete trouvee.\n", id);

}

void sauvegarder_taches(GestionnaireTaches* gt, const char* nom_fichier){
    FILE* fichier = fopen(nom_fichier, "w");
    
    if (fichier == NULL) {
        printf("Erreur : impossible de sauvegarder les taches.\n");
        return;
    }
    
    fprintf(fichier, "%d\n", gt->nb_taches);
    
    // Parcourir toutes les tâches et les écrire dans le fichier
    for (int i = 0; i < gt->nb_taches; i++) {
        fprintf(fichier, "%d|%s|%s|%d|%d|%s\n",
                gt->taches[i].id,
                gt->taches[i].titre,
                gt->taches[i].description,
                gt->taches[i].est_terminee,
                gt->taches[i].est_recurrente,
                gt->taches[i].type_recurrence);
    }
    
    fclose(fichier);
    
    printf("Taches sauvegardees avec succes !\n");
}


void charger_taches(GestionnaireTaches* gt, const char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "r");
    
    if (fichier == NULL) {
        printf("Aucune sauvegarde trouvee.\n");
        return;
    }
    
    
    int nb_taches_sauvegardees;
    fscanf(fichier, "%d\n", &nb_taches_sauvegardees);
    
    for (int i = 0; i < nb_taches_sauvegardees && i < 100; i++) {
        char ligne[1000];  
        
        
        if (fgets(ligne, 1000, fichier) == NULL) {
            break;  
        }
        
        Tache t;
        
        
        char* token = strtok(ligne, "|");
        t.id = atoi(token);  // Convertir en int
        
        token = strtok(NULL, "|");
        strcpy(t.titre, token);
        
        token = strtok(NULL, "|");
        strcpy(t.description, token);
        
        token = strtok(NULL, "|");
        t.est_terminee = atoi(token);
        
        token = strtok(NULL, "|");
        t.est_recurrente = atoi(token);
        
        token = strtok(NULL, "\n");
        if (token != NULL) {
            strcpy(t.type_recurrence, token);
        } else {
            strcpy(t.type_recurrence, "");
        }
        
        
        gt->taches[gt->nb_taches] = t;
        gt->nb_taches++;
    }   
    fclose(fichier);
    printf("%d tache(s) chargee(s) !\n", gt->nb_taches);
}



void supprimer_tache(GestionnaireTaches* g, int id){
    for(int i = 0; i < g->nb_taches; i++){
        if(g->taches[i].id == id){
            for(int j = i; j < g->nb_taches -1; j++){
                g->taches[j] = g->taches[j+1]; //ecrase la tache a supprimer en decalant les autres
            }
            g->nb_taches--; //decremente le nombre de taches
            printf("Tache avec l'ID %d supprimee.\n", id);
            return;
        }
    }
        
    printf("Erreur: aucune tache avec l'ID %d a ete trouvee.\n", id);

}


