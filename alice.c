#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

void sendFile(const char* filename, const char* ip, int port) {
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
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Conexão com o servidor
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao conectar-se ao servidor");
        exit(1);
    }

    // Abertura do arquivo para leitura
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para leitura");
        exit(1);
    }

    // Envio do arquivo para o servidor
    char buffer[MAX_BUFFER_SIZE];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(sockfd, buffer, bytesRead, 0) < 0) {
            perror("Erro ao enviar o arquivo");
            exit(1);
        }
    }

    fclose(file);
    close(sockfd);
    printf("Arquivo enviado com sucesso!\n");
}

int main() {
    const char* filename = "snail.bmp";
    const char* ip = "127.0.0.1";
    int port = 8080;

    sendFile(filename, ip, port);

    return 0;
}
