#include "../headers/main.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <vector>


int main() {
    socklen_t peer_addr_size;
    struct sockaddr_in server_addr, client_addr;
    std::vector<int> connected;
    fd_set fds;
    
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) {
        std::cerr << "Error creating socket";
    };

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi("127.0.0.1"));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int binder = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(binder < 0) {
        std::cerr <<"Error binding socket to adress";
    }

    if(listen(server_socket, 10)) {
        std::cerr << "Error listening socket";
    }


    while(true) {
        FD_ZERO(&fds);
        FD_SET(server_socket, &fds);

        for(std::vector<int>::iterator it = connected.begin(); it != connected.end(); ++it) {
            FD_SET(*it, &fds);
        }

        if(select(FD_SETSIZE + 1, &fds, nullptr, nullptr, nullptr) > 0) {
            if(FD_ISSET(server_socket, &fds)) {
                connected.push_back(accept(server_socket, (struct sockaddr_in *) &server_addr));
            }
        }
    }
}
