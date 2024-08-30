#pragma once
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>


class Upload {
private:
    char* filename_;
    struct stat sb_;
    char* buffer_;

public:
    Upload();
    Upload(const Upload &other);
    ~Upload();
    Upload& operator=(const Upload &other);

    void createFile();
    void returnPath();

    struct stat getSb();
    void setSb(struct stat sb);
    char* getFileName();
    void setFileName(char* filename);
    char* getBuffer();
    void setBuffer(char buffer[]);
};
