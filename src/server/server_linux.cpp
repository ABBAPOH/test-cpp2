#include "server.h"
#include "server_p.h"

#include <iostream>

#include <string.h>
#include <sys/epoll.h>

void ServerPrivate::start()
{
    _kq = epoll_create(10);
    if (_kq == -1) {
        std::cerr << "Can't create epoll" << std::endl;
        return;
    }

    std::cout << "Waiting incoming connections" << std::endl;
    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.events = EPOLLIN /*| EPOLLERR*/ | EPOLLONESHOT;
    ev.data.fd = socket.fd();
    epoll_ctl(_kq, EPOLL_CTL_ADD, socket.fd(), &ev);
}

void ServerPrivate::runOnce()
{
    struct epoll_event ev;
    auto nfd = epoll_wait(_kq, &ev, 1, 100);
    if (!nfd)
        return;

    if (nfd == -1) {
        std::cerr << "epoll failed" << std::endl;
        return;
    }

    if (ev.data.fd == socket.fd()) {
        std::cout << "Accept" << std::endl;
    }
}
