#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <result.h>
#include <atomic>

class TcpSocket
{
public:
    TcpSocket();
    ~TcpSocket();

    Result<void> connect(const std::string &host, int port);
    Result<void> bind(const std::string &host, int port);
    Result<void> listen(int backlog);
    Result<TcpSocket> accept();
    void close();

private:
    explicit TcpSocket(int fd) : _fd(fd) {}
    Result<void> createSocket();

private:
    int _fd {-1};
};

#endif // TCPSOCKET_H
