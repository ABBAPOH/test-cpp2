#ifndef CONNECTION_H
#define CONNECTION_H

#include <tcpsocket.h>

#include <vector>

struct Message;
class ServerPrivate;

class IProcess
{
public:
    virtual ~IProcess() {}
    virtual void process(const Message &message) = 0;
};

class Connection
{
public:
    explicit Connection(const TcpSocket &socket);

    TcpSocket &socket() { return _socket; }

    void process();

    IProcess *handler() const { return _handler; }
    void setHandler(IProcess *handler) { _handler = handler; }

private:
    int64_t readData(const char *data, int64_t length);

private:
    TcpSocket _socket;

    std::vector<char> _readBuffer;
    size_t _readOffset;
    IProcess *_handler {nullptr};
};

#endif // CONNECTION_H
