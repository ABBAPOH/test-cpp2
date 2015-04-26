#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/poll.h>

#include <iostream>

#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"

#include "udplink.h"
#include "optional.h"

int main(int argc, char *argv[])
{
    struct Context {
        Optional<Message> lastMessage;
    };

    if (argc != 3) {
        std::cout << "Usage: ./app host port" << std::endl;
        return 0;
    }

    const auto host = std::string(argv[1]);
    const int port = atoi(argv[2]);

    UdpLink link;
    auto ok = link.init(host, port);
    if (!ok) {
        std::cerr << "Init failed: " << ok.errorString() << std::endl;
        return 1;
    }

    ok = link.open();
    if (!ok) {
        std::cerr << "Open failed: " << ok.errorString() << std::endl;
        return 1;
    }

    auto cb = [](const Message &message, void * data)
    {
        Context &context = *static_cast<Context *>(data);
        if (context.lastMessage && context.lastMessage == message) {
            context.lastMessage = Nothing();
            return;
        }
        std::cout << std::string(message.data().data(), message.size()) << std::endl;
    };

    Context context;
    link.setCallback(cb, &context);

    struct pollfd fds[2];
    fds[0].fd = fileno(stdin);
    fds[0].events = POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI;

    fds[1].fd = link.fd();
    fds[1].events = POLLIN;

    int64_t seq = 0;

    while (1) {
        int r = 0;
        if ((r = poll(fds, 2, 10000)) < 0) {
            std::cerr << "Poll failed: " << strerror(r) << std::endl;
            return 1;
        }

        if (r == 0)
            continue;

        if (fds[0].revents & POLLIN) {
            std::string s;
            std::cin >> s;

            if (s == "/quit")
                return 0;

            Message msg(++seq, ByteArray(s.data(), s.size()));
            ok = link.post(msg);
            if (!ok)
                std::cerr << "Post failed " << ok.errorString() << std::endl;
            context.lastMessage = msg;

        } else if (fds[1].revents & POLLIN) {
            link.process();
        }
    }
}
