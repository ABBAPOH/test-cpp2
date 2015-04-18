#include "server.h"
#include "server_p.h"

#include "message.h"

#include <iostream>

void ServerPrivate::run()
{
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

void ServerPrivate::multiCast(const Message &message)
{
    std::cout << "multiCast" << message.seq << std::endl;
    // TODO: duplicates code in Client::send
    auto bufferSize = sizeof(Message) + message.size;
    std::unique_ptr<char []> buffer(new char[bufferSize]);

    auto msg = reinterpret_cast<Message*>(buffer.get());
    char *d = reinterpret_cast<char *>(msg + 1);

    msg->id = message.id;
//    msg->seq = mess;
    msg->size = message.size;
    msg->data = d;
    memmove(d, message.data, message.size);

//    std::lock_guard<std::mutex> l(connectionMutex);
    for (auto &pair : connections) {
        Connection *connection = pair.second.get();
        auto ok = connection->socket().write(buffer.get(), bufferSize);
        if (!ok)
            std::cerr << "Can't write to client" << ok.errorString();
    }
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

    d->start();

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
