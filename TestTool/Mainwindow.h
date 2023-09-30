#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WebSocketClient.h"
#include <QEventLoop>
#include <QSignalMapper>

QT_BEGIN_NAMESPACE
namespace Ui { class Mainwindow; }
QT_END_NAMESPACE

class Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    void init();
    void Create_Simple_JSON(void);
    void resizeWidget();
    void sleep(int msec)
    {
        QEventLoop loop;
        QTimer::singleShot(msec, &loop, SLOT(quit()));
        loop.exec();
    }

public slots:
    void slotConnect_onClicked();
    bool slotHandleJsonData(QString strData);
    void slotConnectStatus(int nStatus);
    void slotSetDevUniqueId_onClicked();
    void slotSetNICIp_onClicked();
    void slotSendHeart_onClicked();
    void slotNodeATest_onClicked();
    void slotNodeBTest_onClicked();
    void slotRobotEntryTest_onClicked();
    void slotRobotExitTest_onClicked();
    void slotNodeTestButton_onClicked(int nType);
    void slotRobotTestButton_onClicked(int nType);
    void slotFrontCameraA_onClicked();
    void slotBackCamera_onClicked();
    void slotBesideCamera_onClicked();
    void slotFrontCameraB_onClicked();

public:
    Mainwindow(QWidget *parent = nullptr);
    ~Mainwindow();

private:
    Ui::Mainwindow *ui;
    WebSocketClient* m_pWebSocketClient;
    QSignalMapper* m_pSignalMapperNode;
    QSignalMapper* m_pSignalMapperRobot;
    QString m_strDevUniqueID;
    int m_nTrigger;
    int m_nRelay;
    int m_nDisplay;
    int m_nVolume;
    int m_nRadar;
    int m_nRail;
    int m_nLight;
    int m_nRobotDisplay;
    int m_nCtrlLight;
    int m_nNet;
    int m_nAudio;
    int m_nCamera;
    int m_nTelescopicArm;
    int m_nReader;
    int m_nEntryMachine;
    int m_nExitMachine;
    int m_nRobotRelay;
    int m_nScaner;
    int m_nEntryBurn;
    int m_nExitBurn;
};
#endif // MAINWINDOW_H
