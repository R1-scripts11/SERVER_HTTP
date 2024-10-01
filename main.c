#include <stdio.h>      // Pour les fonctions d'entrées/sorties standard (printf, perror)
#include <string.h>     // Pour les fonctions de manipulation de chaînes (strlen, etc.)
#include <unistd.h>     // Pour les fonctions Unix standard (read, write, close)
#include <arpa/inet.h>  // Pour les définitions et fonctions liées aux adresses réseau (AF_INET, sockaddr_in)

int main() {
    // Déclaration des variables pour les sockets
    // Ce sont des int pour les descripteurs de fichiers (identifier par un numéro)
    int server_socket, client_socket;
    // Structures pour stocker l'adresse du serveur et du client
    //struct c'est comme un objet en C de type sockaddr pour stocker les infos sur les adresses réseaux
    struct sockaddr_in server_addr, client_addr;
    // Longueur de l'adresse du client (initialisée à la taille de la structure sockaddr_in)
    socklen_t client_len = sizeof(client_addr);
    // Buffer pour stocker la requête du client
    char buffer[1024];

    // Création du socket serveur
    // AF_INET = Domaine d'adresses Internet (IPv4)
    // SOCK_STREAM = Type de socket pour une connexion TCP (fiable, orientée connexion)
    // 0 = Utilisation du protocole par défaut (TCP)
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Erreur de socket");  // Affiche un message d'erreur en cas d'échec de la création du socket
        return -1;
    }

    // Configuration de l'adresse du serveur
    server_addr.sin_family = AF_INET;  // Utilisation du protocole IPv4
    server_addr.sin_port = htons(8080);  // Numéro de port 8080 (htons = conversion en big-endian)
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Le serveur écoute sur toutes les interfaces réseau disponibles

    // Liaison du socket à l'adresse et au port définis
    // bind() lie le socket à une adresse IP et à un port spécifiques
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur de liaison");  // Affiche un message d'erreur si la liaison échoue
        return -1;
    }

    // Mise en mode écoute du serveur
    // listen() place le socket en état d'écoute, prêt à accepter les connexions
    // 5 est le nombre maximal de connexions en attente (file d'attente des connexions entrantes)
    if (listen(server_socket, 5) < 0) {
        perror("Erreur d'écoute");  // Affiche un message d'erreur si l'écoute échoue
        return -1;
    }

    printf("Serveur HTTP en cours d'exécution sur le port 8080...\n");

    // Boucle infinie pour accepter et traiter les connexions des clients
    while (1) {
        // Acceptation d'une connexion client
        // accept() bloque jusqu'à ce qu'une connexion entrante soit détectée
        // Elle renvoie un nouveau descripteur de socket pour la connexion client
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Erreur d'acceptation");  // Affiche une erreur si l'acceptation échoue
            continue;  // Recommence la boucle pour accepter une nouvelle connexion
        }

        // Lire la requête du client
        // read() lit les données envoyées par le client et les stocke dans le buffer
        read(client_socket, buffer, sizeof(buffer) - 1);  // Lit jusqu'à sizeof(buffer)-1 octets
        printf("Requête reçue : \n%s\n", buffer);  // Affiche la requête HTTP reçue (c'est souvent une requête GET)

        // Construction d'une réponse HTTP simple avec le message "Hello, World!"
        char *http_response =
            "HTTP/1.1 200 OK\r\n"                  // En-tête HTTP avec le code de statut 200 (OK)
            "Content-Type: text/plain\r\n"         // Type de contenu : texte brut
            "Content-Length: 13\r\n"               // Longueur du corps du message : 13 caractères
            "\r\n"                                 // Ligne vide pour séparer les en-têtes du corps
            "Hello, World!";                       // Corps du message (ce qui sera affiché dans le navigateur)
        
        // Envoi de la réponse HTTP au client
        write(client_socket, http_response, strlen(http_response));

        // Fermer la connexion avec le client
        // La connexion est fermée après l'envoi de la réponse
        close(client_socket);
    }

    // Fermer le socket du serveur lorsque la boucle se termine (ce qui n'arrive pas ici car c'est une boucle infinie)
    close(server_socket);
    printf("fermeture du socket");
    return 0;
}
