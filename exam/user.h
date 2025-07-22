#ifndef USER_H
#define USER_H



typedef struct {
    char login[6];
    char password[20];
    char nom[30];
    char prenom[30];
    char telephone[15];
    char role[10];
    int premier_login;
} User;


void ajouter_utilisateur();
int connexion(User *u_connecte);
void changer_password(User *u_connecte);
void menu();

#endif


