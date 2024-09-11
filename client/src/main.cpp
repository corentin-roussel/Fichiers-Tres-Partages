#include "../headers/ClientUpload.hpp"
#include "../headers/main.hpp"

struct User{
    int socket;
    bool isConnected = false;
} user;

int main(int argc, char* argv[]) {
    struct sockaddr_in server_socket;
    int connection;
    char buffer[1024];

    ClientUpload upload = ClientUpload();
    upload.createDirectory();

    if(argc < 2) {
        std::cout << "Please use the command appropriately ./lfp port_number." << std::endl;
        exit(0);
    }

    if((user.socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Problem creating socket" << std::strerror(errno) << std::endl;
        exit(0);
    }

    server_socket.sin_port = htons(atoi(argv[1]));
    server_socket.sin_addr.s_addr = INADDR_ANY;
    server_socket.sin_family = AF_INET;

    connection = connect(user.socket, (struct sockaddr *) &server_socket, sizeof(server_socket));
    if(connection < 0) {
        std::cerr << "Problem connecting to socket " << std::strerror(errno) << std::endl;
        exit(0);
    }

    std::string input;

    while(strcmp(buffer, "quit") != 0)
    {
        if(strcmp(buffer, "-upload") == 0) {
            if(Authentication())
                std::cout << "upload" << std::endl;
        }else if (strcmp(buffer, "-download") == 0) {
            if(Authentication())
                std::cout << "download" << std::endl;
        }
        memset(buffer, 0, sizeof(buffer));
        std::cin >> buffer;

        send(user.socket, buffer, 1024, 0);
        memset(buffer, 0, 1024);
        int receive = recv(user.socket, buffer, 1024, 0);

        buffer[receive] = '\0';
        if(strcmp(buffer, "ping") == 0) {
            std::cout << "pong" << std::endl;
        }
    }

    close(user.socket);
    exit(0);
}

bool Authentication(){
    if(user.isConnected) return true;
    std::string credentials;
    char *input = nullptr;
    char buffer[1024];

    // ask credentials
    std::cout << "Please enter you username and your password in this format : 'username password'" << std::endl;

    // check credentials (server) :

    //envoyer les credentials au server
    memset(buffer, 0, sizeof(buffer));
    std::cin >> credentials;

    //recevoir la reponse du server
    send(user.socket, buffer, sizeof(buffer), 0);         // PAS user.socket mais les crédentials
    memset(buffer, 0, sizeof(buffer));
    int receive = recv(user.socket, buffer, sizeof(buffer), 0);

    if(receive < 0){
        std::cout << "Problem getting the response from the server";
        return false;
    }

    //traiter la réponse
    buffer[receive] = '\0';

    while (true){
        // if good, connect, else ask if the user want to create an account
        if(strcmp(buffer, "login ok") == 0){
            user.isConnected = true;
            std::cout << "You are connected" << std::endl;
            return true;
        }

        std::cout << "This pair username-password does not exist. What do you want to do? (login/register)" << std::endl;
        std::cin >> input;

        if(strcmp(input, "login") == 0){

        }
        else if(strcmp(input, "register") == 0){

        }
        else{
            std::cout << "This pair username-password does not exist. What do you want to do? (login/register)" << std::endl;
        }

        // if yes, ask credentials for register, else ask credentials for login

        // if register, check credentials format

        // send to server and wait for answer
        // - for register check if username don't exist
        // - for login check credentials
    }
}
