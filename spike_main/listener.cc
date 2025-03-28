#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

#define SOCK_PATH "/home/faruk/tmp/spike_socket"

int main() {
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket creation failed");
        return 1;
    }

    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);

    unlink(SOCK_PATH);
    int bind_stat = bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    if (bind_stat == -1) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    int listen_stat = listen(server_fd, 1);
    if (listen_stat == -1) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    char buffer[256];

    std::cout << "Listening for connections...\n";
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd == -1) {
            perror("Accept failed");
            continue;
        }

        ssize_t bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "Received: " << buffer << std::endl;
        }

        close(client_fd);
    }

    close(server_fd);
    unlink(SOCK_PATH);
    return 0;
}
