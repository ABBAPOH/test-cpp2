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
    auto ok = _socket.connect("127.0.0.1", 5001);
    if (ok) {
        _connection.reset(new Connection(_socket));
    }
    return ok;
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

    return connection()->post(*message);
}

