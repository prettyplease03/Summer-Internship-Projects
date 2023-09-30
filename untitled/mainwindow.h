#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QTimer>
#include <QDir>
#include <QProcess>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QWebSocket* webSocket;
    QWebSocketServer* WebSocketServer;
    QTimer *timer;
    QByteArray s;
    QList<QByteArray> cleanData;
    int OBUID;
    int last_cardRestMoney;
    void parseC0(QByteArray cmd);
    void parseC1(QByteArray cmd);
    void parseC6(QByteArray cmd);
    void sendB0();


private slots:
    //void getNewConnection();
    //void ReceivedMsg(const QString &message);
    void take(QAbstractSocket::SocketError error);
    void pushButton_1_clicked();
    void onNewConnection();
    void onBinaryMessageReceived(const QByteArray msg);
    void onDisconnection();
    void sendB2();
    void sendB5();
};


#endif // MAINWINDOW_H
