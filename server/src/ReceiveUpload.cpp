#include "../headers/ReceiveUpload.hpp"

ReceiveUpload::ReceiveUpload() {

}
ReceiveUpload::ReceiveUpload(const ReceiveUpload&other) {
    this->file_name_ = other.file_name_;
}
ReceiveUpload::~ReceiveUpload() {

}
ReceiveUpload& ReceiveUpload::operator=(const ReceiveUpload&other) {
    if(this != &other)
    {
        *this = other;
    }
    return *this;

}
