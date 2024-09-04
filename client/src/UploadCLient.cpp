#include "../headers/ClientUpload.hpp"

ClientUpload::ClientUpload(/* args */)
{
}

ClientUpload::ClientUpload(const ClientUpload&other)
{
    this->filename_ = other.filename_;
}

ClientUpload::~ClientUpload()
{
}

ClientUpload& ClientUpload::operator=(const ClientUpload&other)
{
    if(this != &other)
    {
        *this = other;
    }
    return *this;
}


void ClientUpload::createDirectory() {
    fs::path exe_path = getExePath(getBuffer());
    fs::path file_to_create = getFileToCreate();
    fs::path full_path = exe_path / file_to_create;

    if(!(stat(full_path.c_str(),&sb_) == 0)) {
        std::filesystem::create_directory(full_path);
        std::cout << "directory created" << std::endl;
    }
}

fs::path ClientUpload::getExePath(char *buffer) {
    ssize_t count = readlink("/proc/self/exe", buffer, 1024);
    buffer[count] = 0;
    return fs::path(buffer).parent_path();
}

int ClientUpload::uploadFile(ssize_t fileSize,char *buffer, int chunkSize ,int fileDescriptor) {
    int i = 0;

    while(i < fileSize) {
        int ret = send(fileDescriptor, &buffer[i], __min(getChunkSize(), sizeof(buffer) - i), 0);
        if(ret < 0) { 
            return ret;
        }
        i+=ret;
    }
    return i;
}

ssize_t ClientUpload::getFileSize(const char* pathName) {
    struct stat stat_file;
    ssize_t size_file;
    if(stat(pathName, &stat_file) < 0) {
        std::cerr << "stat struct not initialized. " << std::strerror(errno);
    }
    return size_file = stat_file.st_size;
}



struct stat ClientUpload::getSb() {
    return sb_;
}

void ClientUpload::setSb(struct stat sb) {
    sb_ = sb;
}

char* ClientUpload::getFileName() {
    return filename_;
}

void ClientUpload::setFileName(char* filename) {
    filename_ = filename;
}

char* ClientUpload::getBuffer() {
    return buffer_;
}


const char* ClientUpload::getFileToCreate() {
    return fileToCreate_;
}

void ClientUpload::setFileToCreate(char* fileName) {
     fileToCreate_ = fileName;
}

int ClientUpload::getChunkSize() {
    return chunkSize_;
}
