#ifndef UDPLINK_H
#define UDPLINK_H

#include "message.h"
#include "result.h"

#include <sys/socket.h>
#include <arpa/inet.h>

class UdpLink
{
public:
    typedef void (*Callback)(const Message &message, void *data);

    UdpLink();
    ~UdpLink();

    inline int fd() const { return _fd; }

    Result<void> init(const std::string &host, int port);
    Result<void> open();
    void close();

    Result<void> process();
    Result<void> post(const Message &message);

    // TODO: we can add interface instead of c-style callback
    inline void setCallback(Callback callback, void *data)
    {
        _callback = callback;
        _callbackData = data;
    }

private:
    void processMessage(const Message &message);

    struct sockaddrBuf
    {
        char _buf[128];
        size_t size;
        sockaddrBuf() : size(0) {}

        operator sockaddr * () { return (sockaddr *) _buf; }
        operator const sockaddr * () const { return (const sockaddr *) _buf; }

        sockaddr * operator -> () { return (sockaddr *) _buf; }
        const sockaddr * operator -> () const { return (const sockaddr *) _buf; }

        sockaddr_in * sin() { return (sockaddr_in *) _buf; }
        const sockaddr_in * sin() const { return (const sockaddr_in *) _buf; }

        sockaddr_in6 * sin6() { return (sockaddr_in6 *) _buf; }
        const sockaddr_in6 * sin6() const { return (const sockaddr_in6 *) _buf; }
    };

    std::string _host;
    int _port {0};
    sockaddrBuf _addr;
    int _fd {-1};

    ByteArray _buffer;

    Callback _callback {nullptr};
    void *_callbackData {nullptr};
};

#endif // UDPLINK_H
