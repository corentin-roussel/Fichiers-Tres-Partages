#include <cerrno>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>

int main() {
    int client_socket;
    struct sockaddr_in server_socket;
    socklen_t s_socket_len;
    int connection;
    char buffer[1024];

    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Problem creating socket" << std::strerror(errno) << std::endl;
        exit(0);
    }

    server_socket.sin_port = htons(8080);
    server_socket.sin_addr.s_addr = INADDR_ANY;
    server_socket.sin_family = AF_INET;

    connection = connect(client_socket, (struct sockaddr *) &server_socket, sizeof(server_socket));
    if(connection < 0) {
        std::cerr << "Problem connecting to socket " << std::strerror(errno) << std::endl;
        exit(0);
    }


    while(true)
    {
        memset(buffer, 0, sizeof(buffer));
        std::cin >> buffer;

        int sender = send(client_socket, buffer, 1024, 0);
        memset(buffer, 0, 1024);
        int receive = recv(client_socket, buffer, 1024, 0);

        buffer[receive] = '\0';
        if(strcmp(buffer, "ping") == 0) {
            std::cout << "pong" << std::endl;
        }
        if(strcmp(buffer, "quit") == 0) {
            break;
        }
    }

    close(client_socket);
    exit(0);

}
