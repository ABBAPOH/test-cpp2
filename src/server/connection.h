#ifndef CONNECTION_H
#define CONNECTION_H

#include <tcpsocket.h>

#include <vector>

struct Message;
class ServerPrivate;

class Connection
{
public:
    Connection(ServerPrivate *server, const TcpSocket &socket);

    TcpSocket &socket() { return _socket; }

    void read();

private:
    int64_t readData(const char *data, int64_t length);
    void process(const Message &message);

private:
    ServerPrivate *_server {nullptr};
    TcpSocket _socket;

    std::vector<char> _readBuffer;
    size_t _readOffset;
};

#endif // CONNECTION_H
