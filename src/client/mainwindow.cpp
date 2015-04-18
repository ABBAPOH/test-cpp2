#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "client.h"

#include <message.h>
#include <result.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDebug>
#include <QtCore/QSocketNotifier>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    client(new Client)
{
    ui->setupUi(this);

    connect(ui->actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::send);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::send);

    client->connect();

    auto notifier = new QSocketNotifier(client->fd(), QSocketNotifier::Read, this);
    connect(notifier, &QSocketNotifier::activated, this, &MainWindow::onMessageReceived);

    auto timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &MainWindow::onMessageReceived);
    timer->start();

    _readBuffer.resize(1024);
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

void MainWindow::send()
{
    auto string = ui->lineEdit->text();
    auto utf8 = string.toUtf8();
    qDebug() << "send" << string;

    auto ok = client->send(utf8.constData(), utf8.length());
    if (!ok) {
        QMessageBox::warning(this,
                             tr("Send"),
                             tr("Can't send message: %1").
                             arg(QString::fromStdString(ok.errorString())));
        return;
    }

    ui->lineEdit->clear();
}

// TODO: duplicates code in Connection
void MainWindow::onMessageReceived()
{
//    qDebug() << "Client::recv";

    auto &socket = client->socket();
    auto ok = socket.read(_readBuffer.data() + _readOffset, _readBuffer.size() - _readOffset);

    if (!ok) {
        qDebug() << "Can't read from socket";
//        std::cerr << "Can't read from socket" << ok.errorString() << std::endl;
        return;
    }

    auto length = *ok;
    if (length == 0)
        return;
    qDebug() << length;

    char *data = _readBuffer.data();
    const std::size_t total_length = _readOffset + length;
    const std::size_t bytes_read = readData(data, total_length);
    std::memmove(data, data + bytes_read, total_length - bytes_read);
    _readOffset = total_length - bytes_read;
}

int64_t MainWindow::readData(const char *data, int64_t length)
{
    std::size_t result = 0;
    const char *end = data + length;
    const char *ptr = data;
    while (ptr + sizeof(Frame) <= end) {
        auto *frame = reinterpret_cast<const Frame *>(ptr);
        const size_t fullSize = sizeof(Frame) + frame->size;

        Message msg;
        msg.id = frame->id;
        msg.seq = frame->seq;
        msg.size = frame->size;
        msg.data = const_cast<char *>(reinterpret_cast<const char *>(ptr + sizeof(Frame)));

        if (ptr + fullSize > end)
            break;
        ptr += sizeof(Frame);

        process(msg);

        ptr += frame->size;
        result += fullSize;
    }
    return result;
}

void MainWindow::process(const Message &message)
{
    auto text = ui->textEdit->toPlainText();
    auto line = QString::fromUtf8(message.data, message.size);
    text += "\n" + line;
    ui->textEdit->setPlainText(text);
}
