#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "categorie.h"

#define FICHIER_CATEGORIES "CATEGORIES.dat"

int genererIDCategorie() {
    FILE *f = fopen(FICHIER_CATEGORIES, "rb");
    if (!f) return 1;

    Categorie cat;
    int lastId = 0;
    while (fread(&cat, sizeof(Categorie), 1, f)) {
        if (cat.id > lastId)
            lastId = cat.id;
    }
    fclose(f);
    return lastId + 1;
}

void ajouterCategorie() {
    animationChargement("\x1b[36m🔷 Chargement du module ajout-categorie\x1b[0m");
    FILE *f = fopen(FICHIER_CATEGORIES, "ab");
    if (!f) {
        printf("Erreur ouverture fichier.\n");
        return;
    }

    Categorie cat;
    cat.id = genererIDCategorie();
    printf("Entrez le libellé de la catégorie : ");

    getchar();  // vide le \n laissé par scanf précédent
    fgets(cat.libelle, sizeof(cat.libelle), stdin);
    cat.libelle[strcspn(cat.libelle, "\n")] = 0;  // supprime le \n de fgets

    fwrite(&cat, sizeof(Categorie), 1, f);
    fclose(f);
    printf("Catégorie ajoutée avec succès.\n");
}


/*void listerCategories() {
    FILE *f = fopen(FICHIER_CATEGORIES, "rb");
    if (!f) {
        printf("Aucune categorie trouvee.\n");
        return;
    }

    Categorie cat;
    printf("\nID\tLibelle\n");
    printf("------------------------\n");

    while (fread(&cat, sizeof(Categorie), 1, f)) {
        printf("%d\t%s\n", cat.id, cat.libelle);
    }

    fclose(f);
}*/
void listerCategories() {
     animationChargement("\x1b[36m🔷 Chargement du module liste-categorie\x1b[0m");
    FILE *f = fopen(FICHIER_CATEGORIES, "rb");
    if (!f) {
        printf("\x1b[31mAucune catégorie trouvée.\x1b[0m\n");
        return;
    }
    Categorie cat;
    printf("\n\x1b[36m╔══════════════════════════════════════╗\x1b[0m\n");
    printf("\x1b[32m║         LISTE DES CATEGORIES         ║\x1b[0m\n");
    printf("\x1b[36m╠══════════════════════════════════════╣\x1b[0m\n");
    printf("\x1b[33m║  ID       │ Libellé                  ║\x1b[0m\n");
    printf("\x1b[36m╠══════════════════════════════════════╣\x1b[0m\n");

    int found = 0;
    while (fread(&cat, sizeof(Categorie), 1, f)) {
        printf("\x1b[33m║  %-8d │ %-25s║\x1b[0m\n", cat.id, cat.libelle);
        found = 1;
    }

    if (!found) {
        printf("\x1b[31m║       Aucune catégorie enregistrée   ║\x1b[0m\n");
    }

    printf("\x1b[36m╚══════════════════════════════════════╝\x1b[0m\n");

    fclose(f);
}







void supprimerCategorie() {
     animationChargement("\x1b[36m🔷 Chargement du module supprimer-categorie\x1b[0m");
    FILE *f = fopen(FICHIER_CATEGORIES, "rb");
    FILE *tmp = fopen("TMP.dat", "wb");
    if (!f || !tmp) {
        printf("Erreur d'acces aux fichiers.\n");
        return;
    }

    Categorie cat;
    int id, found = 0;

    printf("Entrez l'ID de la categorie a supprimer : ");
    scanf("%d", &id);

    while (fread(&cat, sizeof(Categorie), 1, f)) {
        if (cat.id != id)
            fwrite(&cat, sizeof(Categorie), 1, tmp);
        else
            found = 1;
    }

    fclose(f);
    fclose(tmp);
    remove(FICHIER_CATEGORIES);
    rename("TMP.dat", FICHIER_CATEGORIES);

    if (found)
        printf("Categorie supprimee.\n");
    else
        printf("Categorie introuvable.\n");
}



Categorie* chercherCategorieParId(int id) {
    FILE *f = fopen(FICHIER_CATEGORIES, "rb");
    if (!f) return NULL;

    static Categorie cat;
    while (fread(&cat, sizeof(Categorie), 1, f)) {
        if (cat.id == id) {
            fclose(f);
            return &cat;
        }
    }
    fclose(f);
    return NULL;
}



/*void menuCategorie() {
    int choix;
    do {
        printf("\n=== GESTION DES CATEGORIES ===\n");
        printf("1. Ajouter une categorie\n");
        printf("2. Supprimer une categorie\n");
        printf("3. Lister les categories\n");
        printf("0. Retour\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: ajouterCategorie(); break;
            case 2: supprimerCategorie(); break;
            case 3: listerCategories(); break;
            case 0: break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 0);
}*/






void menuCategorie() {
    int choix;

    animationChargement("\x1b[36m🔷 Chargement du module Catégories\x1b[0m");

    do {
        printf("\n\x1b[36m╔══════════════════════════════════════╗\x1b[0m\n");
        printf("\x1b[32m║         GESTION DES CATEGORIES       ║\x1b[0m\n");
        printf("\x1b[36m╠══════════════════════════════════════╣\x1b[0m\n");
        printf("\x1b[33m║ 1. Ajouter une catégorie             ║\x1b[0m\n");
        printf("\x1b[33m║ 2. Supprimer une catégorie           ║\x1b[0m\n");
        printf("\x1b[33m║ 3. Lister les catégories             ║\x1b[0m\n");
        printf("\x1b[31m║ 0. Retour                            ║\x1b[0m\n");
        printf("\x1b[36m╚══════════════════════════════════════╝\x1b[0m\n");

        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: ajouterCategorie(); break;
            case 2: supprimerCategorie(); break;
            case 3: listerCategories(); break;
            case 0: break;
            default: printf("\x1b[31mChoix invalide.\x1b[0m\n");
        }
    } while (choix != 0);
}



