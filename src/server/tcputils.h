#ifndef TCPUTILS_H
#define TCPUTILS_H

#include <result.h>

class TcpUtils
{
public:
    TcpUtils() = delete;
    TcpUtils(const TcpUtils &other) = delete;
    TcpUtils &operator =(const TcpUtils &other) = delete;

    static Result<int> open(const std::string &host, int port);
};

#endif // TCPUTILS_H
