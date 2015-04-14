#include "server.h"
#include "server_p.h"

#include <iostream>

Server::Server() :
    d(new ServerPrivate)
{
}

Server::~Server()
{
    delete d;
}

Result<void> Server::init(int threadsCount)
{
    if (threadsCount <= 0)
        return Error(std::string("Invalid threads count") + std::to_string(threadsCount));

    d->threadsCount = threadsCount;

    return Nothing();
}

Result<void> Server::start()
{
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
