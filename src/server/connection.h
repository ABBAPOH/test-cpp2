#ifndef CONNECTION_H
#define CONNECTION_H

#include <tcpsocket.h>

#include <vector>

struct Message;

class Connection
{
public:
    explicit Connection(const TcpSocket &socket);

    void read();

private:
    int64_t readData(const char *data, int64_t length);
    void process(const Message &message);

private:
    TcpSocket _socket;

    std::vector<char> _readBuffer;
    size_t _readOffset;
};

#endif // CONNECTION_H
