#include "tcpsocket.h"

#include <vector>

#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>

namespace Internal {

static std::pair<std::string, std::string> sock2str(const sockaddr *addr, const size_t len)
{
    // Maximum: 8 * 4 + 7 bytes + '\0'
    char host[32 + 7 + 1] = {0};
    char port[6] = {0};
    getnameinfo(addr, len, host, sizeof(host), port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV);
    return std::pair<std::string, std::string>(host, port);
}

int setNonBlock(int fd)
{
    auto flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return errno;
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    return 0;
}

int setNoDelay(int fd)
{
    int flag = 1;
    return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof (flag));
}

} // namespace Internal

using namespace Internal;

TcpSocket::TcpSocket()
{

}

TcpSocket::~TcpSocket()
{

}

Result<void> TcpSocket::connect(const std::string &host, int port)
{
    if (_fd == -1) {
        auto ok = createSocket();
        if (!ok)
            return ok;
    }

    struct addrinfo *addrInfo = 0;
    const struct addrinfo hints = { 0, AF_INET, SOCK_STREAM, 0, 0, 0, 0, 0 };
    if (const int r = getaddrinfo(host.c_str(), 0, &hints, &addrInfo))
        return Error(std::string("Failed to resolve address ") + host + ": " + gai_strerror(r));
    struct addrinfo *addr = addrInfo;
    auto _addr_len = addr->ai_addrlen;

    std::vector<char> _sbuf;
    _sbuf.resize(_addr_len);
    auto _addr = reinterpret_cast<struct sockaddr *>(_sbuf.data());
    memcpy(_addr, addr->ai_addr, _addr_len);
    freeaddrinfo(addrInfo);

    struct sockaddr_in *sin = reinterpret_cast<struct sockaddr_in *>(_addr);
    sin->sin_port = htons(port);

    if (::connect(_fd, _addr, _addr_len))
        return Error(std::string("Connect failed: ") + strerror(errno));

    return Nothing();
}

Result<void> TcpSocket::bind(const std::string &host, int port)
{
    if (_fd == -1) {
        auto ok = createSocket();
        if (!ok)
            return ok;
    }

    struct addrinfo *addrInfo = 0;
    const struct addrinfo hints = { 0, AF_INET, SOCK_STREAM, 0, 0, 0, 0, 0 };
    if (const int r = getaddrinfo(host.c_str(), 0, &hints, &addrInfo))
        return Error(std::string("Failed to resolve address ") + host + ": " + gai_strerror(r));
    struct addrinfo *addr = addrInfo;
    auto _addr_len = addr->ai_addrlen;

    std::vector<char> _sbuf;
    _sbuf.resize(_addr_len);
    auto _addr = (struct sockaddr *) &_sbuf[0];
    memcpy(_addr, addr->ai_addr, _addr_len);
    freeaddrinfo(addrInfo);

    struct sockaddr_in * sin = (struct sockaddr_in *) _addr;
    sin->sin_port = htons(port);

    auto n = sock2str(_addr, _addr_len);
    std::cout << "Resolved " << n.first << ":" << n.second.c_str() << std::endl;

    if (::bind(_fd, _addr, _addr_len))
        return Error(std::string("Bind failed: ") + strerror(errno));

    return Nothing();
}

Result<void> TcpSocket::listen(int backlog)
{
    if (::listen(_fd, backlog))
        return Error(std::string("Listen failed: ") + strerror(errno));

    return Nothing();
}

Result<TcpSocket> TcpSocket::accept()
{
    union {
            char c;
            sockaddr_in6 sin6;
            sockaddr_in sin;
            sockaddr sa;
    } addr;
    socklen_t len = sizeof(addr);

    const int fd = ::accept(_fd, &addr.sa, &len);
    if (fd == -1)
        return Error(std::string("Accept failed :") + strerror(errno));

    return TcpSocket(fd);
}

void TcpSocket::close()
{
    if (_fd != -1) {
        ::close(_fd);
        _fd = -1;
    }
}

Result<int64_t> TcpSocket::read(char *data, int64_t size)
{
    if (size == 0)
        return 0;

    auto bytesRead = ::read(_fd, data, size);
    if (bytesRead == -1)
        return Error(std::string("Read failed: ") + strerror(errno));

    return bytesRead;
}

Result<int64_t> TcpSocket::write(const char *data, int64_t size)
{
    if (size == 0)
        return 0;

    auto bytesWritten = ::write(_fd, data, size);
    if (bytesWritten == -1)
        return Error(std::string("Write failed: ") + strerror(errno));

    return bytesWritten;
}

Result<void> TcpSocket::createSocket()
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return Error(std::string("Failed to create socket: ") + strerror(errno));
    _fd = fd;

    {
        int flag = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof (flag)))
            return Error(std::string("setsockopt failed: ") + strerror(errno));
    }

//    if (setNonBlock(fd))
//        return Error(std::string("setNonBlock failed: ") + strerror(errno));

//    if (setNoDelay(fd))
//        return Error(std::string("setNoDelay failed: ") + strerror(errno));

    return Nothing();
}

