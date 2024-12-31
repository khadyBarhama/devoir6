// serveur.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int counter = 0;

    // Création du socket serveur
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du socket serveur");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Liaison du socket au port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur lors de la liaison du socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Mise en écoute du socket
    if (listen(server_fd, 1) < 0) {
        perror("Erreur lors de l'écoute");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexion sur le port %d...\n", PORT);

    // Acceptation d'une connexion client
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) < 0) {
        perror("Erreur lors de l'acceptation de la connexion client");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connecté.\n");

    while (1) {
        // Envoi du compteur au client
        if (send(client_fd, &counter, sizeof(counter), 0) < 0) {
            perror("Erreur lors de l'envoi du compteur");
            break;
        }

        printf("Envoyé au client : %d\n", counter);

        // Réception du compteur depuis le client
        if (recv(client_fd, &counter, sizeof(counter), 0) < 0) {
            perror("Erreur lors de la réception du compteur");
            break;
        }

        printf("Reçu du client : %d\n", counter);

        // Incrémentation du compteur
        counter++;
        sleep(1);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
