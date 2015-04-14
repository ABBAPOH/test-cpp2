#ifndef SERVER_H
#define SERVER_H

class ServerPrivate;
class Server
{
public:
    Server();
    ~Server();

    bool init(int threadsCount = 4);
    bool start();
    void stop();
    void destroy();

private:
    ServerPrivate *d {nullptr};
};

#endif // SERVER_H
