#ifndef SERVER_P_H
#define SERVER_P_H

#include "server.h"

#include <thread>
#include <vector>

class ServerPrivate
{
public:
    int threadsCount {4};
    std::vector<std::thread> _threads;

    void run() {}
    void stop() {}

    friend class Server;
};

#endif // SERVER_P_H
