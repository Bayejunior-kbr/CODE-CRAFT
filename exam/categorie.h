#ifndef CATEGORIE_H
#define CATEGORIE_H

typedef struct {
    int id;
    char libelle[50];
} Categorie;

int genererIDCategorie();
void ajouterCategorie();
void supprimerCategorie();
void listerCategories();
void menuCategorie();
Categorie* chercherCategorieParId(int id);

#endif
