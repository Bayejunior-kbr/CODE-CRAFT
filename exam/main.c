#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "user.h"

// Couleurs ANSI
#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define CYAN    "\x1b[36m"

void afficherAccueil() {
    system("cls");
    printf(CYAN "╔══════════════════════════════════════════════════════╗\n" RESET);
    printf(CYAN "║" RESET);
    printf(GREEN "            🌟 BIENVENUE SUR SUNUPHARMA 🌟            " RESET);
    printf(CYAN "║\n" RESET);
    printf(CYAN "║" RESET);
    printf(YELLOW "     Logiciel de gestion de stock & ventes pharmacie  " RESET);
    printf(CYAN "║\n" RESET);
    printf(CYAN "╚══════════════════════════════════════════════════════╝\n" RESET);
    printf("\n");
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    while (1) {
        afficherAccueil();

        printf(CYAN "🔐 Veuillez choisir votre rôle pour vous connecter :\n" RESET);
        printf(GREEN "  [1] ADMIN\n" RESET);
        printf(YELLOW "  [2] PHARMACIEN\n" RESET);
        printf(RED "  [0] Quitter\n" RESET);
        printf("\nVotre choix : ");

        int choixRole = -1;
        scanf("%d", &choixRole);

        if (choixRole == 0) {
            printf(GREEN "\nMerci d'avoir utilisé SUNUPHARMA. À bientôt !\n" RESET);
            exit(0);
        }

        char role[6] = "";

        if (choixRole == 1) {
            strcpy(role, "ADMIN");
        } else if (choixRole == 2) {
            strcpy(role, "PHAR");
        } else {
            printf(RED "\n❌ Choix invalide. Veuillez réessayer.\n" RESET);
            Sleep(1500);
            continue;
        }

        User utilisateur_connecte;

        while (!connexion(&utilisateur_connecte) || strcmp(utilisateur_connecte.role, role) != 0) {
            printf(RED "❌ Identifiants invalides ou rôle incorrect. Réessayez.\n" RESET);
        }

        if (utilisateur_connecte.premier_login) {
            changer_password(&utilisateur_connecte);
        }

        printf(GREEN "✅ Bienvenue %s (%s) !\n" RESET, utilisateur_connecte.nom, utilisateur_connecte.role);

        // Appel du menu principal pour ce rôle
        menuPrincipal(utilisateur_connecte.role);

        // Après déconnexion
        printf(YELLOW "\n🔄 Vous avez été déconnecté.\n" RESET);
        Sleep(1500);
    }

    return 0;
}
