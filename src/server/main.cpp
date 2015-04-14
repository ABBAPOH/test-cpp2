#include "server.h"

#include <iostream>

#include <signal.h>

Server *_server = 0;

static void sigIntSandler(int)
{
    std::cerr << "sigint" << std::endl;
    _server->stop();
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    Server server;
    _server = &server;
    auto ok = server.init();
    if (!ok) {
        std::cerr << "Can't init server:" << ok.errorString() << std::endl;
        return 1;
    }

    ok = server.start();
    if (!ok) {
        std::cerr << "Can't init server:" << ok.errorString() << std::endl;
        return 1;
    }

    signal(SIGINT, sigIntSandler);

    server.waitForFinished();

    return 0;
}

