#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

void ajouter_utilisateur() {
     animationChargement("\x1b[36m🔷 Chargement du module ajout-user\x1b[0m");
    FILE *f = fopen("USERS.dat", "ab");
    if (f == NULL) {
        perror("Erreur ouverture fichier");
        return;
    }

    User u;
    printf("Login (5 lettres MAJ) : ");
    scanf("%5s", u.login);

    strcpy(u.password, "pharma123");
    u.premier_login = 1;

    printf("Nom : "); scanf("%s", u.nom);
    printf("Prénom : "); scanf("%s", u.prenom);
    printf("Téléphone : "); scanf("%s", u.telephone);
    printf("Rôle (ADMIN ou PHAR) : ");
    scanf("%s", u.role);

    fwrite(&u, sizeof(User), 1, f);
    fclose(f);
    printf("✅ Utilisateur ajouté avec succès.\n");
}


int connexion(User *u_connecte) {
     animationChargement("\x1b[36m🔷 Chargement du module connection\x1b[0m");
    FILE *f = fopen("USERS.dat", "rb");
    if (f == NULL) {
        perror("Erreur ouverture fichier");
        return 0;
    }

    char login[6], password[20];
    printf("Login : ");
    scanf("%5s", login);
    printf("Mot de passe : ");
    scanf("%s", password);

    User u;
    while (fread(&u, sizeof(User), 1, f)) {
        if (strcmp(u.login, login) == 0 && strcmp(u.password, password) == 0) {
            *u_connecte = u;
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}


void changer_password(User *u_connecte) {
    if (u_connecte->premier_login == 1) {
        char nouveau[20];
        printf("🔒 Vous devez changer votre mot de passe : ");
        scanf("%s", nouveau);

        strcpy(u_connecte->password, nouveau);
        u_connecte->premier_login = 0;

        FILE *f = fopen("USERS.dat", "rb+");
        if (f == NULL) {
            perror("Erreur fichier");
            return;
        }

        User temp;
        while (fread(&temp, sizeof(User), 1, f)) {
            if (strcmp(temp.login, u_connecte->login) == 0) {
                fseek(f, -sizeof(User), SEEK_CUR);
                fwrite(u_connecte, sizeof(User), 1, f);
                break;
            }
        }

        fclose(f);
        printf("✅ Mot de passe modifié avec succès.\n");
    }
}


/*void afficher_utilisateurs() {
     animationChargement("\x1b[36m🔷 Chargement du module affiche-user\x1b[0m");
    FILE *f = fopen("USERS.dat", "rb");
    if (f == NULL) {
        perror("Erreur ouverture fichier");
        return;
    }

    User u;
    printf("\n📄 Liste des utilisateurs :\n");
    while (fread(&u, sizeof(User), 1, f)) {
        printf("🔸 Login: %s | Nom: %s %s | Rôle: %s | Tel: %s\n",
               u.login, u.nom, u.prenom, u.role, u.telephone);
    }

    fclose(f);
}*/

void afficher_utilisateurs() {
    animationChargement("\x1b[36m🔷 Chargement du module affiche-user\x1b[0m");

    FILE *f = fopen("USERS.dat", "rb");
    if (f == NULL) {
        printf("\x1b[31m[ERREUR] Impossible d'ouvrir USERS.dat\x1b[0m\n");
        return;
    }

    User u;

    printf("\n");
    printf("\x1b[36m╔════════════════════════════════════════════════════════════════════════════════╗\x1b[0m\n");
    printf("\x1b[32m║                          👥 LISTE DES UTILISATEURS                             ║\x1b[0m\n");
    printf("\x1b[36m╠════════════╦════════════════════╦════════════════════╦════════════╦════════════╗\x1b[0m\n");
    printf("\x1b[33m║   Login    ║        Nom         ║       Prénom       ║   Rôle     ║ Téléphone  ║\x1b[0m\n");
    printf("\x1b[36m╠════════════╬════════════════════╬════════════════════╬════════════╬════════════╣\x1b[0m\n");

    while (fread(&u, sizeof(User), 1, f) == 1) {
        printf("\x1b[33m║ %-10s ║ %-18s ║ %-18s ║ %-10s ║ %-10s ║\x1b[0m\n",
               u.login, u.nom, u.prenom, u.role, u.telephone);
    }

    printf("\x1b[36m╚════════════╩════════════════════╩════════════════════╩════════════╩════════════╝\x1b[0m\n");

    fclose(f);
}




void menuUtilisateurs() {
    int choix;
    animationChargement("\x1b[36m🔷 Chargement du module Utilisateurs\x1b[0m");

    User utilisateur_connecte;

    do {
        printf("\x1b[36m\n╔══════════════════════════════════════╗\n\x1b[0m");
        printf("\x1b[32m║           MENU UTILISATEURS          ║\n\x1b[0m");
        printf("\x1b[36m╠══════════════════════════════════════╣\n\x1b[0m");
        printf("\x1b[33m║ 1. Ajouter un utilisateur (Admin)    ║\n\x1b[0m");
        printf("\x1b[33m║ 2. Se connecter                      ║\n\x1b[0m");
        printf("\x1b[33m║ 3. Afficher les utilisateurs         ║\n\x1b[0m");
        printf("\x1b[31m║ 0. Retour                            ║\n\x1b[0m");
        printf("\x1b[36m╚══════════════════════════════════════╝\n\x1b[0m");
        printf("Votre choix : ");

        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouter_utilisateur();
                break;
            case 2:
                if (connexion(&utilisateur_connecte)) {
                    printf("Bienvenue %s !\n", utilisateur_connecte.nom);
                    if (utilisateur_connecte.premier_login) {
                        changer_password(&utilisateur_connecte);
                    }
                } else {
                    printf("Échec de la connexion.\n");
                }
                break;
            case 3:
                afficher_utilisateurs();
                break;
            case 0:
                printf("Retour au menu principal.\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
}
