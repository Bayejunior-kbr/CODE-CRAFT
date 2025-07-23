#include <stdio.h>
#include "vente.h"

int main() {
    int choix;

    do {
        printf("\n=== MENU VENTES PHARMACIE ===\n");
        printf("1. Vendre un médicament\n");
        printf("2. Consulter le stock\n");
        printf("3. Imprimer état des ventes du jour\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: effectuerVente(); break;
            case 2: afficherStock(); break;
            case 3: imprimerEtatVentesJour(); break;
            case 0: printf("Fermeture du module Ventes...\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 0);

    return 0;
}
