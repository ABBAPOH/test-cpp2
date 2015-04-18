#include "client.h"

#include <message.h>

Client::Client()
{

}

Client::~Client()
{

}

Result<void> Client::connect()
{
    return _socket.connect("127.0.0.1", 5001);
}

Result<void> Client::send(const char *data, int64_t size)
{
    // TODO: duplicates code in Server::multicast
    auto bufferSize = sizeof(Message) + size;
    std::unique_ptr<char []> buffer(new char[bufferSize]);

    auto message = reinterpret_cast<Message*>(buffer.get());
    char *d = reinterpret_cast<char *>(message + 1);

    message->id = 1;
    message->seq = ++_seq;
    message->size = size;
    message->data = d;
    memmove(d, data, size);

    auto ok = _socket.write(buffer.get(), bufferSize);
    if (!ok)
        return Error(ok.errorString());

    return Nothing();
}

