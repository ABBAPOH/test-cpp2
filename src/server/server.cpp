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

bool Server::init(int threadsCount)
{
    if (threadsCount <= 0) {
        std::cerr << "Invalid threads count" << threadsCount << std::endl;
        return false;
    }

    d->threadsCount = threadsCount;

    return true;
}

bool Server::start()
{
    for (int i = 0; i < d->threadsCount; ++i) {
        d->_threads.push_back(std::thread([this]() {
            d->run();
        } ));
    }

    return true;
}

void Server::stop()
{
    d->stop();

    for (std::thread &thread: d->_threads)
        thread.join();

    d->_threads.clear();
}
