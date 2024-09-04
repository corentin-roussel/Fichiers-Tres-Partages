#include <cerrno>
#include <netinet/in.h>
#include <ostream>
#include <string.h>
#include <string>
#include <sys/select.h>
#include <vector>
#include "../headers/ClientUpload.hpp"

int main(int argc, char* argv[]) {
    int client_socket;
    struct sockaddr_in server_socket;
    int connection;
    char buffer[1024];
    ClientUpload upload = ClientUpload();

    upload.createDirectory();
    ssize_t file_size = upload.getFileSize(argv[3]);
    std::vector<std::string> split = upload.split(argv[1],':');


    std::cout << argv[0] << std::endl;
    std::cout << argv[1] << std::endl;
    std::cout << argv[2] << std::endl;
    std::cout << argv[3] << std::endl;

    if(argc != 4) {
        std::cout << "Please use the command appropriately ./lfp ip:port -type filename" << std::endl;
        exit(0);
    }


    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Problem creating socket" << std::strerror(errno) << std::endl;
        exit(0);
    }

    server_socket.sin_port = htons(atoi(split.at(1).c_str()));
    server_socket.sin_addr.s_addr = INADDR_ANY;
    server_socket.sin_family = AF_INET;

    connection = connect(client_socket, (struct sockaddr *) &server_socket, sizeof(server_socket));
    if(connection < 0) {
        std::cerr << "Problem connecting to socket " << std::strerror(errno) << std::endl;
        exit(0);
    }

    while(true)
    {
        if(strcmp(argv[2], "-upload")) {
            std::cout << "uploading file" << std::endl;
            upload.uploadFile(file_size, upload.getBuffer(), client_socket);

        }else if (strcmp(argv[2], "-download")) {
            std::cout << "downloading file" << std::endl;
        }else {
            std::cout << "cannot compare the argv[2]" << std::endl;
        }
        memset(buffer, 0, sizeof(buffer));
        std::cin >> buffer;

        send(client_socket, buffer, 1024, 0);
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
