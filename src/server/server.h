#ifndef SERVER_H
#define SERVER_H

#include <result.h>

class ServerPrivate;
class Server
{
public:
    Server();
    ~Server();

    Result<void> init(int port, int threadsCount = 4);
    Result<void> start();
    void stop();
    void destroy();

    void waitForFinished();

private:
    ServerPrivate *d {nullptr};
};

#endif // SERVER_H
