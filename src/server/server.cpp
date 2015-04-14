#include "server.h"
#include "server_p.h"

#include <iostream>

void ServerPrivate::run()
{
    _runningThreads++;

    while (!stopped) {
        runOnce();
    }

    if (_runningThreads--) {
        std::unique_lock<std::mutex> l(mutex);
        waitCondition.notify_one();
    }
}

void ServerPrivate::stop()
{
    stopped = true;
}

Server::Server() :
    d(new ServerPrivate)
{
}

Server::~Server()
{
    delete d;
}

Result<void> Server::init(int port, int threadsCount)
{
    if (threadsCount <= 0)
        return Error(std::string("Invalid threads count") + std::to_string(threadsCount));

    d->threadsCount = threadsCount;
    d->port = port;

    return Nothing();
}

Result<void> Server::start()
{
    d->stopped = false;

    auto ok = d->socket.bind("127.0.0.1", d->port);
    if (!ok)
        return Error(ok.errorString());
    ok = d->socket.listen(10);
    if (!ok)
        return Error(ok.errorString());

    for (int i = 0; i < d->threadsCount; ++i) {
        d->_threads.push_back(std::thread([this]() {
            d->run();
        } ));
    }

    return Nothing();
}

void Server::stop()
{
    d->stop();

    for (std::thread &thread: d->_threads)
        thread.join();

    d->_threads.clear();
}

void Server::waitForFinished()
{
    std::unique_lock<std::mutex> l(d->mutex);
    d->waitCondition.wait(l);
}
