#include "../headers/ClientUpload.hpp"
#include "../headers/main.hpp"

struct User{
    int socket;
    bool isConnected = false;
} user;

struct AuthProtocol{
    int codeAction;
    char buffer[128];
    char serverResponse[1024];
} aProtocol;

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
    std::string credentials;
    char input[64];

    if(user.isConnected)
        return true;

    // ask credentials
    std::cout << "Please enter you username and your password in this format : 'username password'" << std::endl;

    // check credentials (server) :

    //envoyer les credentials au server
    aProtocol.codeAction = 1;
    memset(aProtocol.buffer, 0, sizeof(aProtocol.buffer));
    std::cin >> aProtocol.buffer;
    send(user.socket, &aProtocol, sizeof(aProtocol), 0);
    ssize_t receive;

    //recevoir la reponse du server
    memset(aProtocol.serverResponse, 0, sizeof(aProtocol.serverResponse));
    receive = recv(user.socket, aProtocol.serverResponse, sizeof(aProtocol.serverResponse), 0);

    if(receive < 0){
        std::cout << "Problem getting the response from the server";
        return false;
    }

    //traiter la réponse
    aProtocol.serverResponse[receive] = '\0';

    // if good, connect, else ask if the user want to create an account
    if(strcmp(aProtocol.serverResponse, "login ok") == 0){
        user.isConnected = true;
        std::cout << "You are connected" << std::endl;
        return true;
    }
    std::cout << "This pair username-password does not exist. What do you want to do? (login/register)" << std::endl;

    while (true){
        memset(input, 0, sizeof(input));
        std::cin >> input;

        if(strcmp(input, "login") == 0){
            aProtocol.codeAction = 1;
        }
        else if(strcmp(input, "register") == 0){
            aProtocol.codeAction = 2;
        }
        else{
            std::cout << "This command does not exist. What do you want to do? (login/register)" << std::endl;
            aProtocol.codeAction = 0;
            memset(aProtocol.buffer, 0, sizeof(aProtocol.buffer));
            memset(aProtocol.serverResponse, 0, sizeof(aProtocol.serverResponse));
            continue;
        }

        std::cout << "Please enter you username and your password in this format : 'username password'" << std::endl;
        memset(aProtocol.buffer, 0, sizeof(aProtocol.buffer));
        std::cin >> aProtocol.buffer;
        send(user.socket, &aProtocol, sizeof(aProtocol), 0);

        memset(aProtocol.serverResponse, 0, sizeof(aProtocol.serverResponse));
        receive = recv(user.socket, aProtocol.serverResponse, sizeof(aProtocol.serverResponse), 0);

        if(receive < 0){
            std::cout << "Problem getting the response from the server";
            return false;
        }

        //traiter la réponse
        aProtocol.serverResponse[receive] = '\0';

        // if good, connect, else ask if the user want to create an account
        if(strcmp(aProtocol.serverResponse, "login ok") == 0){
            user.isConnected = true;
            std::cout << "You are connected" << std::endl;
            return true;
        }else if(strcmp(aProtocol.serverResponse, "register ok") == 0){
            user.isConnected = true;
            std::cout << "You are registered" << std::endl;
            return true;
        }

        std::cout << "This pair username-password does not exist. What do you want to do? (login/register)" << std::endl;
    }
}
