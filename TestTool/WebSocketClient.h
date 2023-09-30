#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QtWebSockets/QWebSocket>

class WebSocketClient : public QObject
{
    Q_OBJECT

public:
    bool isConnected() { return m_bIsConnect; }
    void setPort(int nPort) { m_nPort = nPort; }
    void setIp(QString strIp) { m_strIp = strIp; }
    void setHeartMsg(QString strMsg) { m_strHeartMsg = strMsg; }
    void connectSvr();
    void disconnect();
    int send(QString strData);

private:
    QWebSocket* m_pSocket;
    QTimer* m_pTimerConnect;
    QTimer* m_pTimerHeart;
    QMutex m_mutex;
    QString m_strHeartMsg;
    QString m_strIp;
    int m_nPort;
    bool m_bIsConnect;

public slots:
    void slotConnected();
    void slotDisconnected();
    void slotReconnect();
    void slotRecv(QString strData);
    void slotSendHeartMsg();

signals:
    void signalConnectStatus(int nStatus);
    void signalRecvJsonData(QString strData);

public:
    WebSocketClient();
    ~WebSocketClient();
};

#endif // WEBSOCKETCLIENT_H
