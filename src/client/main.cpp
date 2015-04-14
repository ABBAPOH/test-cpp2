#include <tcpsocket.h>

#include <iostream>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    TcpSocket socket;
    auto ok = socket.connect("127.0.0.1", 5001);
    if (!ok) {
        std::cerr << "Can't connect: " << ok.errorString();
        return 1;
    }

    while (1) {}

    return 0;
}

