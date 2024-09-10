#include <cstring>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <vector>
#include <unistd.h>
#include "../../transfer/headers/Receive.hpp"
#include "../../transfer/headers/Send.hpp"


int main(int argc, char* argv[]) {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    std::vector<int> connected;
    fd_set fds;
    int client_socket;

    Receive receive = Receive();
    Send sending = Send();


    if(argc < 2) {
        std::cout << "Please use the command appropriately ./lfp_serv port_number." << std::endl;
        exit(0);
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) {
        std::cerr << ("Error creating socket") << std::strerror(errno) << std::endl;
    };
    std::cout << "Socket created" << std::endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << ("Error binding socket") << std::strerror(errno) << std::endl;
        exit(0);
    }
    std::cout << "Socket binded" << std::endl;

    if(listen(server_socket, 10)) {
        std::cerr << ("Error creating socket") << std::strerror(errno) << std::endl;
    }

    while(true) {
        FD_ZERO(&fds);
        FD_SET(server_socket, &fds);

        for(std::vector<int>::iterator it = connected.begin(); it != connected.end(); ++it) {
            FD_SET(*it, &fds);
        }
        std::cout << "stillgood 1" << std::endl;

        if(select(FD_SETSIZE + 1, &fds, nullptr, nullptr, nullptr) > 0) {
            if(FD_ISSET(server_socket, &fds)) {
                client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_len);
                if(client_socket  == -1) {
                    std::cerr << "Error accpeting connection" << std::strerror(errno) << std::endl;
                }else {
                    connected.push_back(client_socket);
                    std::cout << "Connection accepted from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
                    std::cout << "Client: " << client_socket << std::endl;
                }
            }

            for(auto it = connected.begin(); it != connected.end();)
            {
                if(FD_ISSET(*it, &fds)) {
                    int received = receive.receiveFile(*it);
                    if(received == -1)
                    {
                        std::cout <<  "problem with file creation" << std::endl;
                    }else if (received == -2) {
                        std::cout <<  "problem with retrieving fileSize" << std::endl;
                    }else if (received == -3) {
                        std::cout <<  "problem with retrieving fileSize" << std::endl;
                    }
                }

                if(FD_ISSET(*it, &fds)) {
                    int sent = sending.SendFile(*it, "filename.txt");
                    if(sent == -1)
                    {
                        std::cout <<  "problem with file creation" << std::endl;
                    }else if (sent == -2) {
                        std::cout <<  "problem with retrieving fileSize" << std::endl;
                    }else if (sent == -3) {
                        std::cout <<  "problem with retrieving fileSize" << std::endl;
                    }

                }

                ++it;
            }
        }
    }
}
