#ifndef CLIENT_H
#define CLIENT_H

#include <result.h>
#include <tcpsocket.h>

class Client
{
public:
    Client();
    ~Client();

    int fd() { return _socket.fd(); }
    TcpSocket &socket() { return _socket; }

    Result<void> connect();
    Result<void> send(const char *data, int64_t size);

private:
    int64_t _seq {0};
    TcpSocket _socket;
};

#endif // CLIENT_H
