#include "../headers/Receive.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/socket.h>


Receive::Receive() {

}

Receive::Receive(const Receive &other) {
    this->filename_ = other.filename_;
}

Receive::~Receive() {

}

Receive& Receive::operator=(const Receive&other) {
    if(this != &other)
    {
        *this = other;
    }
    return *this;
}

int Receive::receiveBuffer(int fileDescriptor, char* buffer,int bufferSize, int chunkSize) {
    int remainingBytes = 0;
    while(remainingBytes < bufferSize) {
        int bytesReceived =  recv(fileDescriptor, &buffer[remainingBytes], std::min(chunkSize, bufferSize - remainingBytes), 0);

        if(bytesReceived < 0) { return bytesReceived; }
        remainingBytes+=bytesReceived;
    }
    return remainingBytes;
    
}

int64_t Receive::receiveFile(int fileDescriptor, std::string fileName, int chunkSize) {
    std::stringstream fileReceive;
    fileReceive << getExePath(buffer_) << ("files/");
    std::cout << fileReceive.str() << std::endl; 
    if(!fileExists(fileReceive.str())) {

        std::cerr << "File already exists" << std::strerror(errno) <<std::endl;
        exit(0);
    } 
    
    std::cout << fileReceive.str().append(fileName) << std::endl; 
    std::ofstream file(fileReceive.str().append(fileName), std::ofstream::binary);
    if(file.fail()) { return -1; }

    int64_t fileSize;
    if(receiveBuffer(fileDescriptor, reinterpret_cast<char *>(&fileSize), sizeof(fileSize) != sizeof(fileSize))) {
        return -2;
    }

    std::cout << fileSize << std::endl; 

    char *buffer = new char[chunkSize];
    bool errored = false;
    int64_t bytesToDownload = fileSize;
    while(bytesToDownload != 0) {
        int receivedBytes = receiveBuffer(fileDescriptor, buffer, std::min(bytesToDownload, (int64_t)chunkSize));
        if((receivedBytes < 0) || !file.write(buffer, receivedBytes)) { errored = true; break; }
        bytesToDownload-=receivedBytes;
    }
    delete[] buffer;
    file.close();

    return errored ? -3 : fileSize;
}


fs::path Receive::getExePath(char *buffer) {
    ssize_t count = readlink("/proc/self/exe", buffer, 1024);
    buffer[count] = 0;
    return fs::path(buffer).parent_path();
}

std::string Receive::getCurrentDirectory() {
    return fs::current_path();
}

bool Receive::fileExists(std::string fileName) {
    std::string path = getExePath(buffer_);
    path.append("files/");
    std::cout << path << std::endl;

    for(const auto & entry : fs::directory_iterator(path)) {
        if(strcmp(fileName.c_str(), entry.path().c_str())) {
            return false;
        }
    }
    return true;
}
