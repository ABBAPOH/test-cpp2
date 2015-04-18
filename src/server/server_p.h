#ifndef SERVER_P_H
#define SERVER_P_H

#include "connection.h"
#include "server.h"

#include <tcpsocket.h>

#include <atomic>
#include <condition_variable>
#include <thread>
#include <vector>
#include <unordered_map>

class ServerPrivate
{
public:
    void run();
    void start();
    void runOnce();
    void stop();

    void multiCast(const Message &message);

    int threadsCount {4};
    int port {5000};
    TcpSocket socket;

    int _kq {0};

    std::vector<std::thread> _threads;
    std::atomic_int _runningThreads {0};
    std::atomic_bool stopped {false};

    std::mutex mutex;
    std::condition_variable waitCondition;

    std::mutex connectionMutex;
    std::unordered_map<int, std::unique_ptr<Connection>> connections;

    friend class Server;
};

#endif // SERVER_P_H
