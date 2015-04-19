#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "client.h"

#include <connection.h>
#include <message.h>
#include <result.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDebug>
#include <QtCore/QSocketNotifier>
#include <QTimer>

class MainWindow::Handler : public IProcess
{
public:
    explicit Handler(MainWindow *window) : _window(window) {}

    void process(const Message &message) override { _window->process(message); }

private:
    MainWindow *_window {nullptr};
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    client(new Client),
    handler(new Handler(this))
{
    ui->setupUi(this);

    connect(ui->actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::send);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::send);

    auto ok = client->connect();
    client->connection()->setHandler(handler.get());
    if (!ok) {
        QMessageBox::warning(0, tr("bla"), tr("Can't conenct"));
    }

    auto notifier = new QSocketNotifier(client->fd(), QSocketNotifier::Read, this);
    connect(notifier, &QSocketNotifier::activated, this, &MainWindow::onMessageReceived);
}

MainWindow::~MainWindow()
{
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

void MainWindow::onMessageReceived()
{
    client->connection()->process();
    return;
}

void MainWindow::process(const Message &message)
{
    auto text = ui->textEdit->toPlainText();
    auto line = QString::fromUtf8(message.data, message.size);
    text += line + "\n";
    ui->textEdit->setPlainText(text);
}
