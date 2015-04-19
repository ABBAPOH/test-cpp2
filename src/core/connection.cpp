#include "connection.h"

#include "message.h"

#include <cstring>
#include <iostream>

Connection::Connection(const TcpSocket &socket) :
    _socket(socket)
{
    _readBuffer.resize(1024);
    _readOffset = 0;
}

void Connection::process()
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

Result<void> Connection::post(const Message &message)
{
    std::cout << "Connection::post" << message.size << std::endl;
    const auto size = message.size;
    const auto bufferSize = sizeof(Frame) + size;
    std::unique_ptr<char []> buffer(new char[bufferSize]);

    auto frame = reinterpret_cast<Frame*>(buffer.get());
    char *data = reinterpret_cast<char *>(frame + 1);

    frame->id = 1;
    frame->seq = message.seq;
    frame->size = size;
    memmove(data, message.data, size);

    auto ok = _socket.write(buffer.get(), bufferSize);
    if (!ok)
        return Error(ok.errorString());

    return Nothing();
}

int64_t Connection::readData(const char *data, int64_t length)
{
    std::size_t result = 0;
    const char *end = data + length;
    const char *ptr = data;
    while (ptr + sizeof(Frame) <= end) {
        auto *frame = reinterpret_cast<const Frame *>(ptr);
        const size_t fullSize = sizeof(Frame) + frame->size;

        Message msg;
        msg.id = frame->id;
        msg.seq = frame->seq;
        msg.size = frame->size;
        msg.data = const_cast<char *>(reinterpret_cast<const char *>(ptr + sizeof(Frame)));

        if (ptr + fullSize > end)
            break;
        ptr += sizeof(Frame);

        if (_handler)
            _handler->process(msg);

        ptr += frame->size;
        result += fullSize;
    }
    return result;
}
