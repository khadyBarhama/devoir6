#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    int counter;

    // Création du socket client
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du socket client");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Adresse invalide");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // Connexion au serveur
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur lors de la connexion au serveur");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur.\n");

    while (1) {
        // Réception du compteur depuis le serveur
        if (recv(client_fd, &counter, sizeof(counter), 0) < 0) {
            perror("Erreur lors de la réception du compteur");
            break;
        }

        printf("Reçu du serveur : %d\n", counter);

        // Incrémentation du compteur
        counter++;
        sleep(1);

        // Envoi du compteur au serveur
        if (send(client_fd, &counter, sizeof(counter), 0) < 0) {
            perror("Erreur lors de l'envoi du compteur");
            break;
        }

        printf("Envoyé au serveur : %d\n", counter);
    }

    close(client_fd);
    return 0;
}


