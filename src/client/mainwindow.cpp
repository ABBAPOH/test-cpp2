#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <result.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDebug>

class Client
{
public:
    Result<void> send(const char *data, int64_t size) { return Nothing(); }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    client(new Client)
{
    ui->setupUi(this);

    connect(ui->actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::send);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::send);
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
