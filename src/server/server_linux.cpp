#include "server.h"
#include "server_p.h"

#include <iostream>
#include <memory>

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

    auto newFd = 0;
    if (ev.data.fd == socket.fd()) {
        std::cout << "Accept" << std::endl;

        auto accapted = socket.accept();
        if (!accapted) {
            std::cerr << "Accept failed" << accapted.errorString() << std::endl;
            return;
        }

        ev.events = EPOLLIN | EPOLLERR | EPOLLONESHOT;
        epoll_ctl(_kq, EPOLL_CTL_MOD, ev.data.fd, &ev);

        newFd = accapted->fd();

        struct epoll_event empty;
        memset(&empty, 0, sizeof(epoll_event));
        empty.data.fd = newFd;
        epoll_ctl(_kq, EPOLL_CTL_ADD, newFd, &empty);

        std::lock_guard<std::mutex> l(connectionMutex);
        connections.emplace(newFd, std::unique_ptr<Connection>(new Connection(*accapted)));
    } else {
        std::cout << "Recv from client" << std::endl;
        newFd = ev.data.fd;

        std::lock_guard<std::mutex> l(connectionMutex);
        auto connection = connections[newFd].get();
        connection->read();
    }

    struct epoll_event ev2;
    memset(&ev2, 0, sizeof(epoll_event));
    ev2.events = EPOLLIN | EPOLLERR | EPOLLONESHOT;
    ev2.data.fd = newFd;

    epoll_ctl(_kq, EPOLL_CTL_MOD, newFd, &ev2);
}
