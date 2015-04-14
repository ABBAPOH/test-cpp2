#include "server.h"
#include "server_p.h"

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
    //memset(ev0, 0, sizeof(ev0));
    if (!nfd)
            return;
    if (nfd == -1) {
        std::cerr << "kevent" << std::endl;
//            _log->error("kevent");
            return;
    }
    if (!ev->udata && (int)ev->ident == socket.fd()) {
        std::cout << "Accept" << std::endl;

    }
}
