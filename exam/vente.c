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
    int prix;
    int quantite;
    int categorie;
    char datePeremption[11];
} Produit;




typedef struct {
    char code[MAX_CODE];
    char designation[MAX_DESIGNATION];
    int quantite;
    int prix_unitaire;
    int prix_total;
} LigneVente;




/* Génère un identifiant unique pour la vente */
void genererNumeroVente(char *buffer, size_t taille) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, taille, "%Y%m%d%H%M%S", tm_info);
}



/* Simule le login du pharmacien en dur */
void obtenirLogin(char *login) {
    strcpy(login, "PHARM");
}




/* Effectuer une vente */


void effectuerVente(void) {
    animationChargement("\x1b[36m🔷 Chargement du module vendre\x1b[0m");

    FILE *fproduits = fopen("PRODUIT.dat", "r");
    if (!fproduits) {
        printf("\x1b[31m[ERREUR] Impossible d'ouvrir PRODUIT.dat\x1b[0m\n");
        return;
    }

    FILE *ftemp = fopen("TMP_PRODUIT.dat", "w");
    if (!ftemp) {
        printf("\x1b[31m[ERREUR] Impossible de créer TMP_PRODUIT.dat\x1b[0m\n");
        fclose(fproduits);
        return;
    }

    Produit prod;
    LigneVente vente;
    char codeRecherche[MAX_CODE];
    int quantiteVoulue;
    int trouve = 0;

    char numeroVente[20], login[MAX_LOGIN], nomFacture[100];
    int total = 0;

    printf("\x1b[36mCode du médicament à vendre : \x1b[0m");
    scanf("%s", codeRecherche);

    while (fscanf(fproduits, "%s %s %d %d %d %s",
                  prod.code, prod.designation, &prod.prix,
                  &prod.quantite, &prod.categorie, prod.datePeremption) == 6) {

        if (!trouve && strcmp(prod.code, codeRecherche) == 0) {
            trouve = 1;

            printf("\n\x1b[32m=== Détails du médicament ===\x1b[0m\n");
            printf("🩺 Désignation        : \x1b[33m%s\x1b[0m\n", prod.designation);
            printf("💲 Prix unitaire      : \x1b[33m%d XOF\x1b[0m\n", prod.prix);
            printf("📦 Quantité en stock  : \x1b[33m%d\x1b[0m\n", prod.quantite);
            printf("🏷️ Catégorie          : \x1b[33m%d\x1b[0m\n", prod.categorie);
            printf("📅 Date de péremption : \x1b[33m%s\x1b[0m\n", prod.datePeremption);

            printf("\n\x1b[36mQuantité à vendre : \x1b[0m");
            scanf("%d", &quantiteVoulue);

            if (quantiteVoulue > prod.quantite) {
                printf("\x1b[31m[ERREUR] Stock insuffisant.\x1b[0m\n");
                fclose(fproduits);
                fclose(ftemp);
                remove("TMP_PRODUIT.dat");
                return;
            }

            genererNumeroVente(numeroVente, sizeof(numeroVente));
            obtenirLogin(login);

            sprintf(nomFacture, "RECU_%s_001_%s.txt", numeroVente, login);

            FILE *ffacture = fopen(nomFacture, "w");
            if (!ffacture) {
                printf("\x1b[31m[ERREUR] Impossible de créer la facture.\x1b[0m\n");
                fclose(fproduits);
                fclose(ftemp);
                remove("TMP_PRODUIT.dat");
                return;
            }

            vente = (LigneVente) {
                .quantite = quantiteVoulue,
                .prix_unitaire = prod.prix,
                .prix_total = quantiteVoulue * prod.prix
            };
            strcpy(vente.code, prod.code);
            strcpy(vente.designation, prod.designation);
            total = vente.prix_total;

            // FACTURE STYLÉE
            fprintf(ffacture, "\n");
            fprintf(ffacture, "╔════════════════════════════════════════════════╗\n");
            fprintf(ffacture, "║              🧾 FACTURE DE VENTE               ║\n");
            fprintf(ffacture, "╚════════════════════════════════════════════════╝\n");

            fprintf(ffacture, "\nNuméro de vente : %-20s\n", numeroVente);
            fprintf(ffacture, "Pharmacien      : %-20s\n", login);
            fprintf(ffacture, "--------------------------------------------------\n");
            fprintf(ffacture, " Code  │ Désignation         │ Qté │  PU  │ Total\n");
            fprintf(ffacture, "--------------------------------------------------\n");

            fprintf(ffacture, " %-5s │ %-20s │ %3d │ %4d │ %5d\n",
                    vente.code, vente.designation, vente.quantite,
                    vente.prix_unitaire, vente.prix_total);

            fprintf(ffacture, "--------------------------------------------------\n");
            fprintf(ffacture, " TOTAL À PAYER : %33d XOF\n", total);
            fprintf(ffacture, "==================================================\n");
            fclose(ffacture);

            printf("\n\x1b[32m[OK] Vente enregistrée. Facture générée : \x1b[33m%s\x1b[0m\n", nomFacture);

            prod.quantite -= quantiteVoulue;

            // 📄 Enregistrer dans ventes.txt pour le rapport journalier
            FILE *fventes = fopen("ventes.txt", "a");
            if (fventes) {
                fprintf(fventes, "%s %d %d\n", vente.designation, vente.prix_unitaire, vente.quantite);
                fclose(fventes);
            } else {
                printf("\x1b[31m[ERREUR] Impossible d'enregistrer dans ventes.txt\x1b[0m\n");
            }
        }

        // On écrit chaque ligne, mise à jour si trouvé
        fprintf(ftemp, "%s %s %d %d %d %s\n",
                prod.code, prod.designation, prod.prix,
                prod.quantite, prod.categorie, prod.datePeremption);
    }

    fclose(fproduits);
    fclose(ftemp);

    if (!trouve) {
        printf("\x1b[31m[ERREUR] Médicament non trouvé.\x1b[0m\n");
        remove("TMP_PRODUIT.dat");
    } else {
        remove("PRODUIT.dat");
        rename("TMP_PRODUIT.dat", "PRODUIT.dat");
    }
}




/* Affiche le stock disponible */
/*void afficherStock(void) {
    FILE *fproduits = fopen("PRODUIT.dat", "rb");
    if (!fproduits) {
        printf("[ERREUR] Impossible d'ouvrir PRODUIT.dat\n");
        return;
    }

    Produit prod;
    printf("\nCode   | Désignation          | Qté | Prix  | Péremption\n");
    printf("----------------------------------------------------------\n");

    while (fread(&prod, sizeof(Produit), 1, fproduits) == 1) {
        printf("%-6s | %-20s | %-3d | %-5.2f | %s\n",
               prod.code, prod.designation, prod.quantite,
               prod.prix, prod.datePeremption);
    }

    fclose(fproduits);
}*/










/* Placeholder pour rapport du jour */
void imprimerEtatVentesJour(void) {
    printf("[INFO] Fonction rapport du jour à compléter.\n");
}
void afficherStock(void) {
    animationChargement("\x1b[36m🔷 Chargement du module affiche-stock\x1b[0m");

    FILE *fproduits = fopen("PRODUIT.dat", "r");
    if (!fproduits) {
        printf("\x1b[31m[ERREUR] Impossible d'ouvrir PRODUIT.dat\x1b[0m\n");
        return;
    }

    Produit prod;

    printf("\n");
    printf("\x1b[36m╔════════════════════════════════════════════════════════════════════════════╗\x1b[0m\n");
    printf("\x1b[32m║                               📦 STOCK ACTUEL                              ║\x1b[0m\n");
    printf("\x1b[36m╠════════════════════════════════════════════════════════════════════════════╣\x1b[0m\n");
    printf("\x1b[33m║ Code   │ Désignation           │ Prix   │ Qté │ Catégorie │ Péremption     ║\x1b[0m\n");
    printf("\x1b[36m╠════════╪═══════════════════════╪════════╪═════╪═══════════╪════════════════╣\x1b[0m\n");

    while (fscanf(fproduits, "%s %s %d %d %d %s",
                  prod.code, prod.designation, &prod.prix, &prod.quantite, &prod.categorie, prod.datePeremption) == 6) {
        printf("\x1b[33m║ %-6s │ %-21s │ %6d │ %3d │ %-9d │ %-14s ║\x1b[0m\n",
       prod.code, prod.designation, prod.prix, prod.quantite,
       prod.categorie, prod.datePeremption);

    }

    printf("\x1b[36m╚════════════════════════════════════════════════════════════════════════════╝\x1b[0m\n");

    fclose(fproduits);
}



void menuVentes(void) {
    int choix;
             animationChargement("\x1b[36m🔷 Chargement du module ventes\x1b[0m");
    do {
        printf("\n");
        printf("\x1b[36m╔══════════════════════════════════════════════╗\x1b[0m\n");
        printf("\x1b[32m║         📦  MENU VENTES PHARMACIE            ║\x1b[0m\n");
        printf("\x1b[36m╠══════════════════════════════════════════════╣\x1b[0m\n");
        printf("\x1b[33m║ 1. Vendre un médicament                      ║\x1b[0m\n");
        printf("\x1b[33m║ 2. Consulter le stock                        ║\x1b[0m\n");
        printf("\x1b[33m║ 3. Imprimer état des ventes du jour          ║\x1b[0m\n");
        printf("\x1b[31m║ 0. Retour au menu principal                  ║\x1b[0m\n");
        printf("\x1b[36m╚══════════════════════════════════════════════╝\x1b[0m\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                effectuerVente();
                break;
            case 2:
                afficherStock();
                break;
            case 3:
                imprimerEtatVentesJour();
                break;
            case 0:
                printf("\x1b[36m↩️ Retour au menu principal...\x1b[0m\n");
                break;
            default:
                printf("\x1b[31m❌ Choix invalide.\x1b[0m\n");
        }

    } while (choix != 0);
}


