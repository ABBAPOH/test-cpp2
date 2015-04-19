#include <tcpsocket.h>

#include <message.h>

#include <cstring>
#include <iostream>
#include <memory>

#include "mainwindow.h"

#include <QtWidgets/QApplication>

#include "client.h"

Result<void> send(TcpSocket &socket, const Message &message)
{
    auto bufferSize = sizeof(Frame) + message.size;

    std::unique_ptr<char []> buffer(new char[bufferSize]);
    auto frame = reinterpret_cast<Frame *>(buffer.get());
    frame->size = message.size;
    frame->id = message.id;
    frame->seq = message.seq;
    std::memmove(frame + 1, message.data, message.size);

    auto ok = socket.write(buffer.get(), bufferSize);
    if (!ok)
        return Error(ok.errorString());

    return Nothing();
}

class Process : public IProcess
{
    void process(const Message &message)
    {
        std::cout << "client process" << std::string(message.data, message.size) << std::endl;
        return;
    }

};

int main(int argc, char *argv[])
{
//    (void)argc;
//    (void)argv;

//    TcpSocket socket;
//    auto ok = socket.connect("127.0.0.1", 5001);
//    if (!ok) {
//        std::cerr << "Can't connect: " << ok.errorString();
//        return 1;
//    }

//    int seq = 1;

//    while (1) {
//        const char *string = "hello world";

//        Message msg;
//        msg.id = 1;
//        msg.seq = seq++;
//        msg.size = strlen(string);
//        msg.data = const_cast<char *>(string);

//        auto sent = ::send(socket, msg);
//        if (!sent) {
//            std::cerr << "Can't send: " << sent.errorString();
//            return 1;
//        }
//    }

    QApplication app(argc, argv);
    MainWindow w;
    w.show();

//        Process p;
//        Client c;
//        c.connect();
//        c.connection()->setHandler(&p);
//        while (true) {
//            c.send("hello", 5);
//            app.processEvents();
//            c.connection()->process();
//        }


    return app.exec();

}


