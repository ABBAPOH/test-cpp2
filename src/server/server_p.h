#ifndef SERVER_P_H
#define SERVER_P_H

#include "server.h"

#include <tcpsocket.h>

#include <atomic>
#include <condition_variable>
#include <thread>
#include <vector>

class ServerPrivate
{
public:
    void run();
    void runOnce() {}
    void stop();

    int threadsCount {4};
    int port {5000};
    TcpSocket socket;

    std::vector<std::thread> _threads;
    std::atomic_int _runningThreads {0};
    std::atomic_bool stopped {false};

    std::mutex mutex;
    std::condition_variable waitCondition;

    friend class Server;
};

#endif // SERVER_P_H
