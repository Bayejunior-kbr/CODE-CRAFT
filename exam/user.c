#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

void ajouter_utilisateur() {
    FILE *f = fopen("USERS.dat", "ab");
    if (f == NULL) {
        perror("Erreur ouverture fichier USERS.dat");
        return;
    }

    User u;

    printf("\n=== AJOUT D'UN UTILISATEUR ===\n");
    printf("Login (5 lettres MAJUSCULES) : ");
    fgets(u.login, sizeof(u.login), stdin);
    u.login[strcspn(u.login, "\n")] = '\0';

    strcpy(u.password, "pharma123");
    u.premier_login = 1;

    printf("Nom : ");
    fgets(u.nom, sizeof(u.nom), stdin);
    u.nom[strcspn(u.nom, "\n")] = '\0';

    printf("Prénom : ");
    fgets(u.prenom, sizeof(u.prenom), stdin);
    u.prenom[strcspn(u.prenom, "\n")] = '\0';

    printf("Téléphone : ");
    fgets(u.telephone, sizeof(u.telephone), stdin);
    u.telephone[strcspn(u.telephone, "\n")] = '\0';

    printf("Rôle (admin ou pharmacien) : ");
    fgets(u.role, sizeof(u.role), stdin);
    u.role[strcspn(u.role, "\n")] = '\0';

    fwrite(&u, sizeof(User), 1, f);
    fclose(f);
    printf(" Utilisateur ajouté avec succès.\n");
}

int connexion(User *u_connecte) {
    FILE *f = fopen("USERS.dat", "rb");
    if (f == NULL) {
        printf("Erreur ouverture fichier: No such file or directory\n");
        return 0;
    }

    char login[6], password[20];

    printf("\n=== CONNEXION ===\n");
    printf("Login : ");
    fgets(login, sizeof(login), stdin);
    login[strcspn(login, "\n")] = '\0';

    printf("Mot de passe : ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    User temp;
    while (fread(&temp, sizeof(User), 1, f)) {
        if (strcmp(temp.login, login) == 0 && strcmp(temp.password, password) == 0) {
            *u_connecte = temp;
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void changer_password(User *u_connecte) {
    if (u_connecte->premier_login == 1) {
        printf("\n🔐 Vous devez changer votre mot de passe (1ère connexion).\n");
        char nouveau[20];
        printf("Nouveau mot de passe : ");
        fgets(nouveau, sizeof(nouveau), stdin);
        nouveau[strcspn(nouveau, "\n")] = '\0';

        strcpy(u_connecte->password, nouveau);
        u_connecte->premier_login = 0;


        FILE *f = fopen("USERS.dat", "rb+");
        if (f == NULL) {
            perror("Erreur fichier lors de la mise à jour");
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
        printf(" Mot de passe changé avec succès.\n");
    }
}

void menu() {
    int choix;
    User u_connecte;

    do {
        printf("\n====== MENU UTILISATEURS ======\n");
        printf("1. Ajouter un utilisateur\n");
        printf("2. Connexion utilisateur\n");
        printf("0. Quitter le menu utilisateur\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        getchar();

        switch (choix) {
            case 1:
                ajouter_utilisateur();
                break;

            case 2:
                if (connexion(&u_connecte)) {
                    changer_password(&u_connecte);
                    printf(" Connecté en tant que %s (%s)\n", u_connecte.nom, u_connecte.role);
                } else {
                    printf(" Connexion échouée.\n");
                }
                break;

            case 0:
                printf(" Retour au menu principal...\n");
                break;

            default:
                printf(" Choix invalide, réessayez.\n");
        }

    } while (choix != 0);
}
