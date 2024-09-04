#include <stdio.h>
#include <sys/socket.h>
class ReceiveUpload {
    private:
    char *file_name_;
    char *file_path;


    public:
    ReceiveUpload();
    ReceiveUpload(const ReceiveUpload&other);
    ~ReceiveUpload();
    ReceiveUpload& operator=(const ReceiveUpload&other);

    int ;
};
