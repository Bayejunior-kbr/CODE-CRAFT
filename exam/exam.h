typedef struct{
    char code[6];
    char designation[30];
    int prix;
    int quantite;
    int categorie;
    char date_p[20];
}PRODUIT;
void menu();
void menuProduits();
void ajouteproduit();
void modifieproduit();
void supprimeproduit();
void rechercheproduit();
void listeproduit();
void menuprincipal();
