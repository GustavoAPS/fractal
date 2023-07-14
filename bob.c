#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[MAX_BUFFER_SIZE];

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

    // Recebimento do arquivo
    FILE* file = fopen("received.bmp", "wb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para escrita");
        exit(1);
    }

    int bytesRead;
    while ((bytesRead = read(newsockfd, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, bytesRead, file);
    }

    fclose(file);
    close(newsockfd);
    close(sockfd);

    printf("Arquivo recebido com sucesso!\n");

    return 0;
}
