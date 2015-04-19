#ifndef CLIENT_H
#define CLIENT_H

#include <connection.h>
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

    Connection *connection() const { return _connection.get(); }

private:
    int64_t _seq {0};
    TcpSocket _socket;
    std::unique_ptr<Connection> _connection;
};

#endif // CLIENT_H
