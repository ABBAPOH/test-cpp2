#include "server.h"

#include <iostream>

#include <signal.h>

Server *_server = 0;

static void sigIntSandler(int)
{
    std::cerr << "Stopping server" << std::endl;
    _server->stop();
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    Server server;
    _server = &server;
    std::cout << "Initializing server" << std::endl;
    auto ok = server.init();
    if (!ok) {
        std::cerr << "Can't init server:" << ok.errorString() << std::endl;
        return 1;
    }

    std::cout << "Starting server" << std::endl;
    ok = server.start();
    if (!ok) {
        std::cerr << "Can't init server:" << ok.errorString() << std::endl;
        return 1;
    }

    std::cout << "Waiting for interrupt" << std::endl;

    signal(SIGINT, sigIntSandler);

    server.waitForFinished();

    std::cout << "Server stopped" << std::endl;

    return 0;
}

