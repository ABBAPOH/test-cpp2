#include "connection.h"

#include <cstring>
#include <iostream>

Connection::Connection(const TcpSocket &socket) :
    _socket(socket)
{
    _readBuffer.resize(1024);
    _readOffset = 0;
}

void Connection::read()
{
    auto ok = _socket.read(_readBuffer.data() + _readOffset, _readBuffer.size() - _readOffset);

    if (!ok) {
        std::cerr << "Can't read from socket" << ok.errorString() << std::endl;
        return;
    }

    auto length = *ok;

    char *data = _readBuffer.data();
    const std::size_t total_length = _readOffset + length;
    const std::size_t bytes_read = readData(data, total_length);
    std::memmove(data, data + bytes_read, total_length - bytes_read);
    _readOffset = total_length - bytes_read;
}

int64_t Connection::readData(const char *data, int64_t length)
{
    int l = 5;
    const char *ptr = data;
    int64_t result = 0;
    while (length >= l) {
        std::cout << "Got mesage of length " << l << " : " << std::string(data, 5) << std::endl;
        ptr += l;
        length -= l;
        result += l;
    }
    return result;
}
