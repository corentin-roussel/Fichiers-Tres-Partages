#pragma once
#include <iostream>
#include <cstring>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <sys/types.h>
#include <unistd.h>

namespace fs = std::filesystem;

class Upload {
private:
    char* filename_;
    struct stat sb_;
    char buffer_[1024];
    const char* fileToCreate_ = "files/";

public:
    Upload();
    Upload(const Upload &other);
    ~Upload();
    Upload& operator=(const Upload &other);

    void uploadFile();
    void downloadFile();
    void createDirectory();
    int getFileSize(const char* filename);
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
};
