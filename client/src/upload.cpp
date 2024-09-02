#include "../headers/upload.hpp"
#include <filesystem>

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

void Upload::uploadFile() {
    
}

int Upload::getFileSize(const char* pathName) {
    FILE *file = fopen(pathName, "wb");
    if( file  == NULL) { 
        std::cerr << "Error cannot find file. " << std::strerror(errno);
    }
    return 1;
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
