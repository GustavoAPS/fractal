#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Criação do socket TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar o socket");
        exit(1);
    }

    // Configuração do endereço do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Conexão com o servidor
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao conectar-se ao servidor");
        exit(1);
    }

    char message[1024];

    // Envio de mensagem para o servidor
    strcpy(message, "Olá, Bob!");
    send(sockfd, message, strlen(message), 0);
    printf("Mensagem enviada para Bob: %s\n", message);

    // Recebimento de resposta do servidor
    memset(message, 0, sizeof(message));
    recv(sockfd, message, sizeof(message), 0);
    printf("Resposta de Bob: %s\n", message);

    // Fechamento do socket
    close(sockfd);

    return 0;
}
