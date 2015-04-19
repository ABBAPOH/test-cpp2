#include "server.h"
#include "server_p.h"

#include "message.h"

#include <iostream>

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

void ServerPrivate::start()
{
    _kq = kqueue();
    if (_kq == -1) {
        std::cerr << "Can't create kqueue" << std::endl;
        return;
    }

    struct kevent ev0[16];
    struct kevent *ev_ch = ev0;

    std::cout << "Waiting incoming connections" << std::endl;
    EV_SET(ev_ch++, socket.fd(), EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, 0);

    int nfd = kevent(_kq, ev0, ev_ch - ev0, 0, 0, 0);
    if (nfd == -1) {
        std::cerr << "kevent" << std::endl;
    }
}

void ServerPrivate::runOnce()
{
    struct timespec waitspec = { 0, 500000 };

    struct kevent ev[1];
    struct kevent ev0[16];
    struct kevent *ev_ch = ev0;

    assert(ev_ch - ev0 < 8);
    int nfd = kevent(_kq, 0, 0, ev, 1, &waitspec);
    ev_ch = ev0;
    if (!nfd)
            return;
    if (nfd == -1) {
        std::cerr << "kevent" << std::endl;
        return;
    }

    int newFd = 0;
    if (!ev->udata && (int)ev->ident == socket.fd()) {
        std::cout << "Accept" << std::endl;

        struct kevent ev;
        memset(&ev, 0, sizeof(struct kevent));

        EV_SET(ev_ch++, socket.fd(), EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, 0);

        auto accepted = socket.accept();
        if (!accepted) {
            std::cerr << "Accept failed" << accepted.errorString() << std::endl;
            return;
        }
        newFd = accepted->fd();

        std::lock_guard<std::mutex> l(connectionMutex);
        auto connection = new Connection(*accepted);
        connection->setHandler(_handler.get());
        connections.emplace(newFd, std::unique_ptr<Connection>(connection));

        Message msg;
        msg.seq = 10;
        msg.id = 1;
        msg.size = 5;
        msg.data = "hello";
        multiCast(msg);

    } else {
        std::cout << "Recv from client" << std::endl;
        newFd = (int)ev->ident;

        std::lock_guard<std::mutex> l(connectionMutex);
        Connection *connection = connections[newFd].get();
        if (ev->flags & EV_ERROR || ev->flags & EV_EOF) {
            std::cerr << "Error event" << std::endl;
            connection->socket().close();
            return;
        }
        connection->process();
    }

    EV_SET(ev_ch++, newFd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, 0);

    nfd = kevent(_kq, ev0, ev_ch - ev0, 0, 0, 0);
    if (nfd == -1) {
        std::cerr << "kevent" << std::endl;
    }
}
