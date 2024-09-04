#pragma once
#include <iostream>
#include <linux/limits.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <unistd.h>
#include <limits.h>
#include <vector>
#define __min(a,b)  (((int)(a) < (int)(b)) ? (a) : (b))

namespace fs = std::filesystem;

class ClientUpload {
private:
    char* filename_;
    struct stat sb_;
    char buffer_[1024];
    char *pathToFile;
    const char* fileToCreate_ = "files/";
    int chunkSize_ = 4*1024;

public:
    ClientUpload();
    ClientUpload(const ClientUpload &other);
    ~ClientUpload();
    ClientUpload& operator=(const ClientUpload&other);

    int uploadFile(ssize_t fileSize, char *buffer, int fileDescriptor);
    void createDirectory();
    ssize_t getFileSize(const char* filename);
    char *getExePath();
    fs::path getExePath(char *buffer);
    std::vector<std::string> split(const std::string& str, char delimiter);

    struct stat getSb();
    void setSb(struct stat sb);
    char* getFileName();
    void setFileName(char* filename);
    char* getBuffer();
    void setBuffer(char buffer[]);
    const char* getFileToCreate();
    void setFileToCreate(char * fileName);
    int getChunkSize();
};
