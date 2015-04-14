#include "tcputils.h"

#include <iostream>
#include <vector>

#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

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

/*!
    Creates socket, setups it's parameters and starts listening.

    Return file descriptor of the created socket
*/

Result<int> TcpUtils::open(const std::string &host, int port)
{
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

    auto fd = socket(_addr->sa_family, SOCK_STREAM, 0);
    if (fd < 0)
        return Error(std::string("Failed to create socket: ") + strerror(errno));

    {
        int flag = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof (flag)))
            return Error(std::string("setsockopt failed: ") + strerror(errno));
    }

    if (setNonBlock(fd))
        return Error(std::string("setNonBlock failed: ") + strerror(errno));

    if (setNoDelay(fd))
        return Error(std::string("setNoDelay failed: ") + strerror(errno));

    if (bind(fd, _addr, _addr_len))
        return Error(std::string("Bind failed: ") + strerror(errno));

    if (listen(fd, 10))
        return Error(std::string("Listen failed: ") + strerror(errno));

    return fd;
}
