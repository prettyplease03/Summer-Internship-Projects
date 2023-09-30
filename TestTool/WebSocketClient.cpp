#include "WebSocketClient.h"
#include "GlobalDefine.h"
#include "iostream"
#include <QJsonObject>
#include <QJsonDocument>

WebSocketClient::WebSocketClient():
    m_strIp(""),
    m_nPort(0),
    m_bIsConnect(false)
{
    m_pSocket = new QWebSocket();
    m_pTimerConnect = new QTimer();
    m_pTimerHeart = new QTimer();

    connect(m_pSocket, &QWebSocket::connected, this, &WebSocketClient::slotConnected);
    connect(m_pSocket, &QWebSocket::disconnected, this, &WebSocketClient::slotDisconnected, Qt::QueuedConnection);
    connect(m_pSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::slotRecv);
    connect(m_pTimerConnect, &QTimer::timeout, this, &WebSocketClient::slotReconnect);
    connect(m_pTimerHeart, &QTimer::timeout, this, &WebSocketClient::slotSendHeartMsg);
}

WebSocketClient::~WebSocketClient()
{
    m_pTimerConnect->stop();
    m_pTimerHeart->stop();
    m_pSocket->abort();

    delete m_pSocket;
    delete m_pTimerConnect;
    delete m_pTimerHeart;
}

void WebSocketClient::connectSvr()
{
    if(m_bIsConnect)
    {
        m_pSocket->abort();
        m_pSocket->close();
    }

    QString strUrl = QString("ws://%1:%2").arg(m_strIp).arg(m_nPort);
    m_pSocket->open(QUrl(strUrl));
}

void WebSocketClient::disconnect()
{
    m_pSocket->abort();
    m_pSocket->close();
}

int WebSocketClient::send(QString strData)
{
    if(!m_bIsConnect)
    {
        qDebug()<<"WebSocket ?????????????????";
        return Enum_NoConnect;
    }

    if(strData.isEmpty())
    {
        qWarning()<<"????????????";
        return Enum_DataNull;
    }

    if(!m_mutex.tryLock(10))
        return Enum_LockFailed;

    qDebug()<<"????????? " + strData;
    int nRet = m_pSocket->sendTextMessage(strData);

    m_mutex.unlock();

    return nRet;
}

void WebSocketClient::slotConnected()
{
    qDebug()<<QString("????????????? %1:%2").arg(m_strIp).arg(m_nPort);

    m_bIsConnect = true;
    m_pTimerConnect->stop();
    m_pTimerHeart->start(HEART_INTERVAL);

    emit signalConnectStatus(1);
}

void WebSocketClient::slotDisconnected()
{
    qDebug()<<"WebSocket ??????";

    m_bIsConnect = false;
    m_pTimerHeart->stop();
    m_pTimerConnect->start(RECONNECT_INTERVAL);

    emit signalConnectStatus(-1);
}

void WebSocketClient::slotReconnect()
{
    qDebug()<<"...????????????????????...";
    QString strUrl = QString("ws://%1:%2").arg(m_strIp).arg(m_nPort);
    m_pSocket->abort();
    m_pSocket->open(QUrl(strUrl));

}

void WebSocketClient::slotRecv(QString strData)
{
    qDebug()<<"?????????" + strData;
    emit signalRecvJsonData(strData);
}

void WebSocketClient::slotSendHeartMsg()
{
    send(m_strHeartMsg);
}








