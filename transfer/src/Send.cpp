#include "../headers/Send.hpp"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sys/types.h>

Send::Send() {

}

Send::Send(const Send &other) {
    this->filename_ = other.filename_;
}

Send::~Send() {

}

Send& Send::operator=(const Send&other) {
    if(this != &other)
    {
        *this = other;
    }
    return *this;
}


void Send::createDirectory() {
    fs::path exe_path = getExePath(buffer_);
    fs::path file_to_create = getFileToCreate();
    fs::path full_path = exe_path / file_to_create;

    if(!(stat(full_path.c_str(),&sb_) == 0)) {
        std::filesystem::create_directory(full_path);
        std::cout << "directory created" << std::endl;
    }
}

fs::path Send::getExePath(char *buffer) {
    ssize_t count = readlink("/proc/self/exe", buffer, 1024);
    buffer[count] = 0;
    return fs::path(buffer).parent_path();
}

int Send::sendBuffer(int fileDescriptor, char* buffer,int bufferSize, int chunkSize) {
    int i = 0;
    while(i < bufferSize) {
        std::cout <<  i <<std::endl;
        int ret = send(fileDescriptor, &buffer[i], std::min(chunkSize, bufferSize - i), 0);
        if(ret < 0) {
            return ret;
        }
        i+=ret;
    }
    return i;
}

int64_t Send::SendFile(int fileDescriptor, const char* fileName, int chunkSize) {
    ssize_t fileSize =  getFileSize(fileName);
    if(fileSize < 0) { return -1; }

    std::ifstream file(fileName, std::ifstream::binary);
    if(file.fail()) { return -1; }


    if(sendBuffer(fileDescriptor, reinterpret_cast<char*>(&fileSize),  sizeof(fileSize) != sizeof(fileSize))) {
        return -2;
    }

    char* buffer = new char[chunkSize];
    bool errored = false;
    int64_t remainingBytes = fileSize;
    while(remainingBytes != 0) {
        const int64_t bytesToRead = std::min(remainingBytes, (int64_t)chunkSize);
        if(!file.read(buffer, bytesToRead)) {errored = true; break;}
        const int bytesSent = sendBuffer(fileDescriptor, buffer, bytesToRead);
        if(bytesSent< 0) { errored = true; break; }
        remainingBytes-=bytesSent;
    }
    delete[]buffer;
    file.close();
    return errored ? -3 :fileSize;
};

ssize_t Send::getFileSize(const char* pathName) {
    struct stat stat_file;
    ssize_t size_file;
    if(stat(pathName, &stat_file) < 0) {
        std::cerr << "stat struct not initialized. " << std::strerror(errno);
    }
    return size_file = stat_file.st_size;
}

std::vector<std::string> Send::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

struct stat Send::getSb() {
    return sb_;
}

void Send::setSb(struct stat sb) {
    sb_ = sb;
}

char* Send::getFileName() {
    return filename_;
}

void Send::setFileName(char* filename) {
    filename_ = filename;
}

char* Send::getBuffer() {
    return buffer_;
}


const char* Send::getFileToCreate() {
    return fileToCreate_;
}

void Send::setFileToCreate(char* fileName) {
     fileToCreate_ = fileName;
}
