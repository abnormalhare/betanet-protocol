#include <arpa/inet.h>
#include <cstdio>
#include <netinet/in.h>
#include <sodium/core.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

enum ERR_SERVER_CONNECTION {
    OK,
    ERR_SERVER_CONNECTION_SOCKET,
    ERR_SERVER_CONNECTION_CONNECT, 
};

int g_skt = 0;

ERR_SERVER_CONNECTION connect_to_server() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return ERR_SERVER_CONNECTION_SOCKET; }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        perror("connect");
        return ERR_SERVER_CONNECTION_CONNECT;
    }

    g_skt = sock;
}

void send_data(const char *msg) {
    send(g_skt, msg, strlen(msg), 0);
}

int main() {
    ERR_SERVER_CONNECTION err = connect_to_server();
    if (err) return 1;

    if (sodium_init() < 0) {
        perror("sodium");
        return 1;
    }

    send_data("HELLO WORLD");
    
    close(g_skt);

    return 0;
}
