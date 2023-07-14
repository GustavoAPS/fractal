#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char message[1024];

    // Criação do socket TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar o socket");
        exit(1);
    }

    // Configuração do endereço do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Associação do socket com o endereço do servidor
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao associar o socket com o endereço do servidor");
        exit(1);
    }

    // Aguardar por conexões
    listen(sockfd, 5);
    printf("Aguardando por conexões...\n");

    // Aceitar conexões
    client_len = sizeof(client_addr);
    newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
    if (newsockfd < 0) {
        perror("Erro ao aceitar a conexão");
        exit(1);
    }
    printf("Conexão estabelecida com Alice.\n");

    // Recebimento de mensagem do cliente
    memset(message, 0, sizeof(message));
    recv(newsockfd, message, sizeof(message), 0);
    printf("Mensagem recebida de Alice: %s\n", message);

    // Resposta ao cliente
    strcpy(message, "Olá, Alice!");
    send(newsockfd, message, strlen(message), 0);
    printf("Resposta enviada para Alice: %s\n", message);

    // Fechamento dos sockets
    close(newsockfd);
    close(sockfd);

    return 0;
}
