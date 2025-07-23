/* ===== DEBUT vente.c ===== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vente.h"

#define MAX_CODE 6
#define MAX_LOGIN 6
#define MAX_DESIGNATION 50

typedef struct {
    char code[MAX_CODE];
    char designation[MAX_DESIGNATION];
    float prix;
    int quantite;
    char categorie[30];
    char datePeremption[11];
} Produit;

typedef struct {
    char code[MAX_CODE];
    char designation[MAX_DESIGNATION];
    int quantite;
    float prix_unitaire;
    float prix_total;
} LigneVente;

void genererNumeroVente(char *buffer, size_t taille) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, taille, "%Y%m%d%H%M%S", tm_info);
}

void obtenirLogin(char *login) {
    strcpy(login, "PHARM");
}

void effectuerVente(void) {
    FILE *fproduits;
    Produit prod;
    int trouve = 0;
    char codeRecherche[MAX_CODE];
    int quantiteVoulue;
    char numeroVente[15];
    char login[MAX_LOGIN];
    char nomFacture[100];
    float total = 0;
    LigneVente vente;
    
    printf("Code du medicament a vendre : ");
    scanf("%s", codeRecherche);

    fproduits = fopen("PRODUCTS.dat", "rb+");
    if (fproduits == NULL) {
        printf("[ERREUR] Impossible d'ouvrir PRODUCTS.dat
");
        return;
    }
    while (fread(&prod, sizeof(Produit), 1, fproduits) == 1) {
        if (strcmp(prod.code, codeRecherche) == 0) {
            trouve = 1;
            printf("Designation : %s
", prod.designation);
            printf("Prix : %.2f
", prod.prix);
            printf("Quantite en stock : %d
", prod.quantite);
            printf("Date de peremption : %s
", prod.datePeremption);

            printf("Quantite a vendre : ");
            scanf("%d", &quantiteVoulue);

            if (quantiteVoulue > prod.quantite) {
                printf("[ERREUR] Stock insuffisant.
");
                fclose(fproduits);
                return;
            }

            genererNumeroVente(numeroVente, sizeof(numeroVente));
            obtenirLogin(login);

            sprintf(nomFacture, "RECU_%s_001_%s.txt", numeroVente, login);

            FILE *ffacture = fopen(nomFacture, "w");
            if (ffacture == NULL) {
                printf("[ERREUR] Impossible de creer le fichier de facture.
");
                fclose(fproduits);
                return;
            }

            strcpy(vente.code, prod.code);
            strcpy(vente.designation, prod.designation);
            vente.quantite = quantiteVoulue;
            vente.prix_unitaire = prod.prix;
            vente.prix_total = quantiteVoulue * prod.prix;
            total = vente.prix_total;

            fprintf(ffacture, "=== FACTURE DE VENTE ===
");
            fprintf(ffacture, "Numero de vente : %s
", numeroVente);
            fprintf(ffacture, "Pharmacien : %s
", login);
            fprintf(ffacture, "------------------------------------
");
            fprintf(ffacture, "Code    Designation         Qte   PU    Total
");
            fprintf(ffacture, "%-6s %-18s %-5d %-6.2f %-7.2f
", vente.code, vente.designation, vente.quantite, vente.prix_unitaire, vente.prix_total);
            fprintf(ffacture, "------------------------------------
");
            fprintf(ffacture, "TOTAL : %.2f XOF
", total);
            fclose(ffacture);

            printf("[OK] Vente enregistree. Facture generee sous le nom : %s
", nomFacture);

            prod.quantite -= quantiteVoulue;
            fseek(fproduits, -sizeof(Produit), SEEK_CUR);
            fwrite(&prod, sizeof(Produit), 1, fproduits);

            fclose(fproduits);
            return;
        }
    }
    if (!trouve) {
        printf("[ERREUR] Medicament non trouve.
");
        fclose(fproduits);
    }
}

void afficherStock(void) {
    FILE *fproduits;
    Produit prod;
    int i = 1;

    fproduits = fopen("PRODUCTS.dat", "rb");
    if (fproduits == NULL) {
        printf("[ERREUR] Impossible d'ouvrir PRODUCTS.dat
");
        return;
    }
    printf("
Code   | Designation          | Qte | Prix | Peremption
");
    printf("-----------------------------------------------------------
");
    while (fread(&prod, sizeof(Produit), 1, fproduits) == 1) {
        printf("%-6s | %-20s | %-3d | %-5.2f | %s
", prod.code, prod.designation, prod.quantite, prod.prix, prod.datePeremption);
        i++;
    }
    fclose(fproduits);
}

void imprimerEtatVentesJour(void) {
    printf("[INFO] Fonction de rapport du jour a completer selon besoin.
");
}
/* ===== FIN vente.c ===== */
