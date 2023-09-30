#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebSocket>
#include <iostream>
#include <sstream>
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
    int i = 0;
    ~MainWindow();
    QByteArray s;
    QList<QByteArray> cleanData;
    int heart_count;
    int disconnect_loop_count;

public:
    void init_cob();
    bool checkForHeart();
    void reconnectToServer();

private slots:
    void connectUrl(QString url);
    void connectButton_clicked();
    void sendCommand_c0();
    void sendCommand_c1();
    void sendCommand_c2();
    void sendCommand_c4();
    void sendCommand_c6();
    void onBinaryMessageReceived(QByteArray msg);
    void onTimeOut();

private:
    Ui::MainWindow *ui;
    QWebSocket *webSocket;
    QUrl m_url;
    QTimer *six_sec_timer;
    QTimer *reconnect;
    QProcess *process;
    int initialBalance;
    int moneyAfterDeduction;
    int OBUID;
    void parse_b2(QByteArray msg);
    void parse_b5(QByteArray msg);
};
#endif // MAINWINDOW_H
