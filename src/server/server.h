#ifndef SERVER_H
#define SERVER_H

#include <result.h>

class ServerPrivate;
class Server
{
public:
    Server();
    ~Server();

    Result<void> init(int threadsCount = 4);
    Result<void> start();
    void stop();
    void destroy();

private:
    ServerPrivate *d {nullptr};
};

#endif // SERVER_H
