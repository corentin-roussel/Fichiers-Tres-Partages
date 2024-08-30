#include "../headers/upload.hpp"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <unistd.h>

Upload::Upload(/* args */)
{
}

Upload::Upload(const Upload &other)
{
    this->filename = other.filename;
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

void Upload::createFile() {

}

void Upload::returnPath() {
    readlink("/home/corentin/dev_logiciel/Fichiers-Tres-Partages/files/", , size_t len)
    std::filesystem path = std::filesystem::current_path();
    const char *path_char = path.string().c_str();
    if(stat(path, &sb_) == 0) {
        
    }else {
        std::filesystem::create_directory("files");
    }
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

void Upload::setBuffer(char buffer[]) {
    buffer_ = buffer[1024];
}
