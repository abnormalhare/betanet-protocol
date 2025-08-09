#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <sodium.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return 1; }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    address.sin_port = htons(12345);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) { perror("bind"); return 1; }
    if (listen(server_fd, 1) < 0) { perror("listen"); return 1; }

    std::cout << "Server listening on port 12345...\n";

    socklen_t addrlen = sizeof(address);
    int client_fd = accept(server_fd, (sockaddr*)&address, &addrlen);
    if (client_fd < 0) { perror("accept"); return 1; }

    char buffer[1024] = {0};
    ssize_t bytes = read(client_fd, buffer, sizeof(buffer));
    if (bytes > 0) {
        std::cout << "Received: " << buffer << "\n";
    }

    close(client_fd);
    close(server_fd);
}
