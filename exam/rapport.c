#include <stdio.h>
#include <time.h>
#include <windows.h>

// Structure pour une vente
typedef struct {
    char nom[30];
    int prix;
    int quantite;
} Vente;

// Charger les ventes depuis ventes.txt
int chargerVentes(Vente ventes[], int max) {
    FILE *fichier = fopen("ventes.txt", "r");
    if (!fichier) {
        printf("\033[1;31m[ERREUR] Impossible d'ouvrir ventes.txt.\033[0m\n");
        return 0;
    }

    int i = 0;
    while (i < max && fscanf(fichier, "%s %d %d", ventes[i].nom, &ventes[i].prix, &ventes[i].quantite) == 3) {
        i++;
    }

    fclose(fichier);
    return i;
}


// Générer rapport journalier
int genererRapport() {
    printf("\033[1;33m[*] Génération du rapport");
    for (int i = 0; i < 3; i++) {
        Sleep(300);
        printf(".");
        fflush(stdout);
    }
    printf("\033[0m\n");

    // Date
    char nomFichier[100];
    time_t maintenant = time(NULL);
    struct tm *date = localtime(&maintenant);
    strftime(nomFichier, sizeof(nomFichier), "ETAT_%Y%m%d.txt", date);

    // Chargement des ventes
    Vente ventes[50];
    int nb = chargerVentes(ventes, 50);

    float total = 0;
    int quantiteTotale = 0;
    for (int i = 0; i < nb; i++) {
        total += ventes[i].prix * ventes[i].quantite;
        quantiteTotale += ventes[i].quantite;
    }

    // Écriture fichier rapport
    FILE *fichier = fopen(nomFichier, "w");
    if (!fichier) {
        printf("\033[1;31m[ERREUR] Impossible de créer le rapport.\033[0m\n");
        return 1;
    }

    fprintf(fichier, "=========== RAPPORT JOURNALIER ===========\n");
    fprintf(fichier, "Date : %02d/%02d/%d\n", date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);
    fprintf(fichier, "Total : %.2f XOF\n", total);
    fprintf(fichier, "Médicaments vendus : %d\n", quantiteTotale);
    fprintf(fichier, "===========================================\n");
    fclose(fichier);

    // Résumé écran
    printf("\n\033[1;36m+=============================+\033[0m\n");
    printf("\033[1;36m|       RAPPORT DU JOUR       |\033[0m\n");
    printf("\033[1;36m+=============================+\033[0m\n");
    printf("Date          : %02d/%02d/%d\n", date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);
    printf("Total ventes  : %.2f XOF\n", total);
    printf("Articles vendus : %d\n\n", quantiteTotale);

    // Tableau ventes
  printf("\033[1;36m╔════════════════════════╦════════╦══════════╗\033[0m\n");  // Cyan bordure haut
printf("\033[1;36m║ Médicament             ║ Prix   ║ Quantité ║\033[0m\n");  // Cyan en-tête
printf("\033[1;36m╠════════════════════════╬════════╬══════════╣\033[0m\n");  // Cyan séparateur
for (int i = 0; i < nb; i++) {
    printf("\033[1;36m║ \033[0m\033[1;37m%-22s\033[0m \033[1;36m║ \033[0m\033[1;37m%6d\033[0m \033[1;36m║ \033[0m\033[1;37m%8d\033[0m \033[1;36m║\033[0m\n",
           ventes[i].nom,
           ventes[i].prix,
           ventes[i].quantite);
}
printf("\033[1;36m╚════════════════════════╩════════╩══════════╝\033[0m\n");  // Cyan bordure bas


    printf("\033[1;32m[OK] Rapport journalier enregistré dans %s.\033[0m\n", nomFichier);
    return 0;
}






void menuRapport() {
    int choix;

    do {
        printf("\n\033[1;36m╔═══════════════════════════════╗\033[0m\n");
        printf("\033[1;36m║      SUNUPHARMA - RAPPORT     ║\033[0m\n");
        printf("\033[1;36m╠═══════════════════════════════╣\033[0m\n");
        printf("\033[1;36m║ 1. Générer le rapport         ║\033[1;36m\n");
        printf("\033[1;31m║ 0. Retour                     ║\033[0m\n");
        printf("\033[1;36m╚═══════════════════════════════╝\033[0m\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n');  // vider le buffer entrée
            printf("\033[1;31m[ERREUR] Entrée invalide. Veuillez saisir un nombre.\033[0m\n");
            choix = -1; // force la boucle à continuer
            continue;
        }

        switch (choix) {
            case 1:
                genererRapport();
                break;

            case 0:
                printf("\033[1;32m[OK] Retour au menu principal.\033[0m\n");
                break;

            default:
                printf("\033[1;31m[ERREUR] Choix invalide.\033[0m\n");
                break;
        }

    } while (choix != 0);
}
