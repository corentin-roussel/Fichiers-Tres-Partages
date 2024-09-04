#pragma once
#include <iostream>
#include <cstring>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#define __min(a,b)  (((a) < (b)) ? (a) : (b))

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

    int uploadFile(ssize_t fileSize, char *buffer, int chunkSize, int fileDescriptor);
    void downloadFile();
    void createDirectory();
    ssize_t getFileSize(const char* filename);
    char *getExePath();
    fs::path getExePath(char *buffer);

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
