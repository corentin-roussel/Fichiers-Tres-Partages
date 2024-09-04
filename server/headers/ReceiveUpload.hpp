#include <stdio.h>
#include <sys/socket.h>
class ReceiveUpload {
    private:
    char *file_name_;


    public:
    ReceiveUpload();
    ReceiveUpload(const ReceiveUpload&other);
    ~ReceiveUpload();
    ReceiveUpload& operator=(const ReceiveUpload&other);
};
