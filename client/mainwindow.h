#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebSocket>
#include <QWebSocketServer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void onConnected();

    void onTextMsg(const QString &msg);

    void onDisconnected();

    void sendData();

    void ListenBu();

    void ListenBu_2();

    void connectUrl(QString url);

    void sendTextMsg(const QString &message);
private:
    Ui::MainWindow *ui;

    QWebSocket* webSocket;
    QUrl m_url;
    bool m_bConnected = false;
};
#endif // MAINWINDOW_H
