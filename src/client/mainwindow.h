#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class Client;
class Connection;
struct Message;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void send();
    void onMessageReceived();

private:
    void process(const Message &message);

private:
    class Handler;

    Ui::MainWindow *ui {nullptr};
    std::unique_ptr<Client> client;
    std::unique_ptr<Handler> handler;
};

#endif // MAINWINDOW_H
