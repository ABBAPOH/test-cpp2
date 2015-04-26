#include "udplink.h"

#include "frame.h"

#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>

UdpLink::UdpLink()
{
    _buffer.resize(1024);
}

UdpLink::~UdpLink()
{

}

Result<void> UdpLink::init(const std::string &host, int port)
{
    if (host.empty())
        return Error("Group can't be empty");

    if (port <= 0)
        return Error("Port can't be less than or equal to zero");

    _host = host;
    _port = port;

    return Nothing();
}

Result<void> UdpLink::open()
{
    struct addrinfo * result = 0;
    const struct addrinfo hints = { 0, AF_INET, SOCK_DGRAM };
    int r = getaddrinfo(_host.c_str(), 0, &hints, &result);
    if (r)
        return Error("Failed to resolve address " + _host + " : " + gai_strerror(r));
    struct addrinfo *addr = result;
    _addr.size = addr->ai_addrlen;
    memcpy(_addr, addr->ai_addr, _addr.size);
    freeaddrinfo(result);

    struct sockaddr_in * sin = _addr.sin();
    sin->sin_port = htons(_port);

    if ((_fd = socket(_addr->sa_family, SOCK_DGRAM, 0)) < 0)
        return Error(std::string("Can't create socket: ") + strerror(errno));

    {
        u_int flag = 1;
        // allow multiple sockets to use the same PORT number
        if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0) {
            close();
            return Error(std::string("Can't set socket option : ") + strerror(errno));
        }
    }

    {
        u_int flag = 1;
        if (setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof (flag)) < 0) {
            if (errno != ENOPROTOOPT) {
                close();
                return Error(std::string("Can't set socket option : ") + strerror(errno));
            }
        }
    }

    {
        struct sockaddr_in sa = { 0 };
        sa.sin_family = AF_INET;
        sa.sin_port = sin->sin_port;
        //        sa.sin_addr = in4addr_any;
        if (bind(_fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
            close();
            return Error(std::string("Bind failed: ") + strerror(errno));
        }
    }

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(_host.c_str());
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        close();
        return Error(std::string("Can't set socket option : ") + strerror(errno));
    }

    return Nothing();
}

void UdpLink::close()
{
    ::close(_fd);
    _fd = -1;
}

Result<void> UdpLink::process()
{
    Frame frame;
    struct sockaddr_in6 sin6 = {0};
    struct iovec iov[2] = {{&frame, sizeof(Frame)}, {_buffer.data(), _buffer.size()}};
    struct msghdr msg = { &sin6, sizeof(sin6), iov, 2 };
    int r = recvmsg(_fd, &msg, MSG_DONTWAIT);

    if (r < 0)
        return Error(std::string("recvmsg failed: ") + strerror(errno));

    Message message;
    message.setSeq(frame.seq);
    ByteArray data(_buffer.data(), r - iov[0].iov_len);
    message.setData(data);

    processMessage(message);

    return Nothing();
}

Result<void> UdpLink::post(const Message &message)
{
    Frame frame;
    struct iovec iov[2] = {{&frame, sizeof(Frame)}, {(void*)message.data().data(), message.size()}};

    struct msghdr m = { _addr, (socklen_t)_addr.size, iov, 2 };
    int r = sendmsg(_fd, &m, MSG_DONTWAIT);

    if (r < 0)
        return Error(std::string("Failed to post: sendmsg failed: ") + strerror(errno));

    return Nothing();
}

void UdpLink::processMessage(const Message &message)
{
    if (_callback)
        _callback(message, _callbackData);
}

