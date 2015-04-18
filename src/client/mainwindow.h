#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class Client;
class Message;

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
    int64_t readData(const char *data, int64_t length);
    void process(const Message &message);

private:
    Ui::MainWindow *ui {nullptr};
    Client *client {nullptr};

    std::vector<char> _readBuffer;
    size_t _readOffset {0};
};

#endif // MAINWINDOW_H
