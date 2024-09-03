#include "../headers/upload.hpp"
#include <cstdio>
#include <filesystem>
#include <linux/limits.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

Upload::Upload(/* args */)
{
}

Upload::Upload(const Upload &other)
{
    this->filename_ = other.filename_;
}

Upload::~Upload()
{
}

Upload& Upload::operator=(const Upload &other)
{
    if(this != &other)
    {
        *this = other;
    }
    return *this;
}


void Upload::createDirectory() {
    fs::path exe_path = getExePath(getBuffer());
    fs::path file_to_create = getFileToCreate();
    fs::path full_path = exe_path / file_to_create;

    if(!(stat(full_path.c_str(),&sb_) == 0)) {
        std::filesystem::create_directory(full_path);
        std::cout << "directory created" << std::endl;
    }
}

fs::path Upload::getExePath(char *buffer) {
    ssize_t count = readlink("/proc/self/exe", buffer, 1024);
    buffer[count] = 0;
    return fs::path(buffer).parent_path();
}

int Upload::uploadFile(ssize_t fileSize,char *buffer, int chunkSize ,int fileDescriptor) {
    int i = 0;

    while(i < fileSize) {
        int error = send(fileDescriptor, &buffer[i], __min(getChunkSize(), sizeof(buffer) - i), 0);
        if(error < 0) { 
            return error;
        }
        i+=error;
    }
    return i;
}

ssize_t Upload::getFileSize(const char* pathName) {
    struct stat stat_file;
    ssize_t size_file;
    if(stat(pathName, &stat_file) < 0) {
        std::cerr << "stat struct not initialized. " << std::strerror(errno);
    }
    return size_file = stat_file.st_size;
}



struct stat Upload::getSb() {
    return sb_;
}

void Upload::setSb(struct stat sb) {
    sb_ = sb;
}

char* Upload::getFileName() {
    return filename_;
}

void Upload::setFileName(char* filename) {
    filename_ = filename;
}

char* Upload::getBuffer() {
    return buffer_;
}


const char* Upload::getFileToCreate() {
    return fileToCreate_;
}

void Upload::setFileToCreate(char* fileName) {
     fileToCreate_ = fileName;
}

int Upload::getChunkSize() {
    return chunkSize_;
}
