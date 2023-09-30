#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include "RobotDataParse.h"
#include "GlobalDefine.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QJsonArray>

Mainwindow::Mainwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Mainwindow)
    , m_strDevUniqueID("")
{
    ui->setupUi(this);

    m_pWebSocketClient = new WebSocketClient();
    m_pSignalMapperNode = new QSignalMapper(this);
    m_pSignalMapperRobot = new QSignalMapper(this);

    init();
    resizeWidget();
    Create_Simple_JSON();

}

Mainwindow::~Mainwindow()
{
    delete m_pWebSocketClient;
    delete ui;
}
#include <QDesktopWidget>
#include <QFont>

void Mainwindow::resizeWidget()
{
    int deskWidth = QApplication::desktop()->screenGeometry().width();
    int deskheight = QApplication::desktop()->screenGeometry().height();
    float baseWidth = 1920;
    float baseHeight = 1080;
    float horRatio = deskWidth/baseWidth;
    float verRatio = deskheight/baseHeight;
    int nFontSizeTitle = deskWidth <= 1024 ? 9 : 11;

    if(deskWidth == 1920 && deskheight == 1080)
        return;

    this->resize(deskWidth, deskheight);

    QList<QLabel*> labelList = ui->centralwidget->findChildren<QLabel*>();
    for(auto it = labelList.begin(); it!=labelList.end(); it++)
    {
        QLabel* pLabel = *it;
        pLabel->setMinimumHeight(20);
        pLabel->setMinimumWidth(0);
    }

    QList<QLineEdit*> lineEditList = ui->centralwidget->findChildren<QLineEdit*>();
    for(auto it = lineEditList.begin(); it!=lineEditList.end(); it++)
    {
        QLineEdit* plineEdit = *it;
        plineEdit->setMinimumHeight(20);
        plineEdit->setMinimumWidth(0);
    }

    QList<QPushButton*> buttonList = ui->centralwidget->findChildren<QPushButton*>();
    for(auto it = buttonList.begin(); it!=buttonList.end(); it++)
    {
        QPushButton* pButton = *it;

        if("button_NodeATest" == pButton->objectName() || "button_NodeBTest" == pButton->objectName() ||
           "button_RobotEntryTest" == pButton->objectName() || "button_RobotExitTest" == pButton->objectName())
        {
            pButton->setMaximumSize(141*horRatio, 61*horRatio);
            pButton->setMinimumSize(141*horRatio, 61*horRatio);
            QFont font;
            font.setPointSize(nFontSizeTitle);
            pButton->setFont(font);
            continue;
        }

        if(pButton->objectName().contains("robotModeCardMachine") || pButton->objectName().contains("robotModeCardMachine") ||
           pButton->objectName().contains("robotModeCardMachine") || pButton->objectName().contains("robotModeCardMachine"))
        {
            pButton->setMinimumSize(101*horRatio, 40*verRatio);
            QFont font;
            font.setPointSize(nFontSizeTitle);
            pButton->setFont(font);

        }

        pButton->setMinimumHeight(20);
        pButton->setMinimumWidth(0);
    }

    QList<QGroupBox*> groupList = ui->centralwidget->findChildren<QGroupBox*>();
    for(auto it = groupList.begin(); it!=groupList.end(); it++)
    {
        QGroupBox* pGroupBox = *it;
        if("groupBox_27" == pGroupBox->objectName())
        {
            QRect resizeRect = pGroupBox->geometry();
            pGroupBox->setGeometry(resizeRect.x()*horRatio, resizeRect.y()*verRatio, 50, 50);
            continue;
        }

        QRect resizeRect = pGroupBox->geometry();
        pGroupBox->setGeometry(resizeRect.x()*horRatio, resizeRect.y()*verRatio, resizeRect.width()*horRatio, resizeRect.height()*verRatio);
    }

    QRect rectNoGroup;

    rectNoGroup = ui->stackedWidget_Main->geometry();
    ui->stackedWidget_Main->setGeometry(rectNoGroup.x()*horRatio+10, rectNoGroup.y()*verRatio, rectNoGroup.width()*horRatio, rectNoGroup.height()*verRatio);

    rectNoGroup = ui->button_sendHeart->geometry();
    ui->button_sendHeart->setGeometry(rectNoGroup.x()*horRatio+10, rectNoGroup.y()*verRatio, rectNoGroup.width()*horRatio, rectNoGroup.height()*verRatio);

    rectNoGroup = ui->textBrowser_log->geometry();
    ui->textBrowser_log->setGeometry(rectNoGroup.x()*horRatio, rectNoGroup.y()*verRatio, rectNoGroup.width()*horRatio, rectNoGroup.height()*verRatio);

    rectNoGroup = ui->textBrowser_recv->geometry();
    ui->textBrowser_recv->setGeometry(rectNoGroup.x()*horRatio, rectNoGroup.y()*verRatio, rectNoGroup.width()*horRatio, rectNoGroup.height()*verRatio);

    rectNoGroup = ui->label_log->geometry();
    ui->label_log->setGeometry(rectNoGroup.x()*horRatio, rectNoGroup.y()*verRatio, rectNoGroup.width()*horRatio, rectNoGroup.height()*verRatio);

    rectNoGroup = ui->label_recv->geometry();
    ui->label_recv->setGeometry(rectNoGroup.x()*horRatio, rectNoGroup.y()*verRatio, rectNoGroup.width()*horRatio, rectNoGroup.height()*verRatio);

    return;

}

void Mainwindow::init()
{
    m_nTrigger = -1;
    m_nRelay = -1;
    m_nDisplay = -1;
    m_nVolume = -1;
    m_nRadar = -1;
    m_nRail = -1;
    m_nLight = -1;
    m_nRobotDisplay = -1;
    m_nCtrlLight = -1;
    m_nNet = -1;
    m_nAudio = -1;
    m_nCamera = -1;
    m_nTelescopicArm = -1;
    m_nReader = -1;
    m_nEntryMachine = -1;
    m_nExitMachine = -1;
    m_nRobotRelay = -1;
    m_nScaner = -1;
    m_nEntryBurn = -1;
    m_nExitBurn = -1;

    connect(m_pWebSocketClient, &WebSocketClient::signalConnectStatus, this , &Mainwindow::slotConnectStatus);
    connect(m_pWebSocketClient, &WebSocketClient::signalRecvJsonData, this, &Mainwindow::slotHandleJsonData);
    connect(ui->button_connect, &QPushButton::clicked, this , &Mainwindow::slotConnect_onClicked);
    connect(ui->button_setDevUniqueId, &QPushButton::clicked, this , &Mainwindow::slotSetDevUniqueId_onClicked);
    connect(ui->button_setNICIp, &QPushButton::clicked, this , &Mainwindow::slotSetNICIp_onClicked);
    connect(ui->button_sendHeart, &QPushButton::clicked, this, &Mainwindow::slotSendHeart_onClicked);
    connect(ui->button_NodeATest, &QPushButton::clicked, this , &Mainwindow::slotNodeATest_onClicked);
    connect(ui->button_NodeBTest, &QPushButton::clicked, this , &Mainwindow::slotNodeBTest_onClicked);
    connect(ui->button_RobotEntryTest, &QPushButton::clicked, this , &Mainwindow::slotRobotEntryTest_onClicked);
    connect(ui->button_RobotExitTest, &QPushButton::clicked, this , &Mainwindow::slotRobotExitTest_onClicked);
    connect(ui->button_frontCameraA, &QPushButton::clicked, this , &Mainwindow::slotFrontCameraA_onClicked);
    connect(ui->button_backCamera, &QPushButton::clicked, this , &Mainwindow::slotBackCamera_onClicked);
    connect(ui->button_besideCamera, &QPushButton::clicked, this , &Mainwindow::slotBesideCamera_onClicked);
    connect(ui->button_frontCameraB, &QPushButton::clicked, this , &Mainwindow::slotFrontCameraB_onClicked);
    connect(ui->button_trigger1_A, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_trigger2_A, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_trigger3_A, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_trigger4_A, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_trigger5_A, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_trigger6_A, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_trigger1_B, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_trigger2_B, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_trigger3_B, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_trigger4_B, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_trigger5_B, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_trigger6_B, SIGNAL(clicked()), m_pSignalMapperNode, SLOT(map()));
    connect(ui->button_relay1Open_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay1Close_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay2Open_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay2Close_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay3Open_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay3Close_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay4Open_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay4Close_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay5Open_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay5Close_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay6Open_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay6Close_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay7Open_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay7Close_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay8Open_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay8Close_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay1Open_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay1Close_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay2Open_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay2Close_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay3Open_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay3Close_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay4Open_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay4Close_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay5Open_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay5Close_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay6Open_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay6Close_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay7Open_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay7Close_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay8Open_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_relay8Close_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_displayColor_Red_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_displayColor_Green_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_displayColor_Yellow_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_displayColor_Blue_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_displayColor_Red_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_displayColor_Green_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_displayColor_Yellow_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_displayColor_Blue_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_volumeTest_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_volumeTest_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_radarTest_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_radarTest_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_railingUp_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_railingDown_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_railingUp_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_railingDown_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_frontLightOpen_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_frontLightClose_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_frontLightOpen_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_frontLightClose_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_besideLightOpen_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_besideLightClose_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_backLightOpen_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_backLightClose_A, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_besideLightOpen_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_besideLightClose_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_backLightOpen_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_backLightClose_B, SIGNAL(clicked()), m_pSignalMapperNode , SLOT(map()));
    connect(ui->button_displayColor_Red_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_displayColor_Green_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_displayColor_Yellow_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_displayColor_Blue_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_displayColor_Red_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_displayColor_Green_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_displayColor_Yellow_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_displayColor_Blue_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight1Open_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight1Close_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight2Open_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight2Close_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight3Open_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight3Close_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight1Open_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight1Close_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight2Open_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight2Close_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight3Open_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_ctrlLight3Close_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_to4GNet_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_toLineNet_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_to4GNet_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_toLineNet_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_audioTest_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_record_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_playRecord_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_audioTest_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_record_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_playRecord_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_openUpCamera_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_closeUpCamera_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_openDownCamera_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_closeDownCamera_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_openUpCamera_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_closeUpCamera_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_openDownCamera_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_closeDownCamera_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_telescopicArmInit_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_telescopicArmTake10_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_telescopicArmTake20_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_telescopicArmBack_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_telescopicArmInit_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_telescopicArmTake10_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_telescopicArmTake20_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_telescopicArmBack_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_openScaner_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_closeScaner_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_openScaner_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_closeScaner_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay1Open_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay1Close_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay2Open_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay2Close_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay3Open_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay3Close_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay4Open_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay4Close_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay1Open_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay1Close_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay2Open_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay2Close_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay3Open_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay3Close_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay4Open_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_relay4Close_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_takeETCAntenna_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_takeCPCAntenna_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotWriteCard_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotReadCard_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotStop_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotWriteCard_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotReadCard_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotStop_Entry, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_takeETCAntenna_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_takeCPCAntenna_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotWriteCard_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotReadCard_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotStop_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotWriteCard_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotReadCard_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotStop_Exit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_robotModeCardMachine, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_sentryboxModeCardMahine, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upleftSlotInit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upleftSlotPrepareCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upleftSlotSendCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upleftSlotGetCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upleftSlotRecoverCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upleftSlotSetCardNum, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_uprightSlotInit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_uprightSlotPrepareCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_uprightSlotSendCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_uprightSlotGetCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_uprightSlotRecoverCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_uprightSlotSetCardNum, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downleftSlotInit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downleftSlotPrepareCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downleftSlotSendCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downleftSlotGetCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downleftSlotRecoverCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downleftSlotSetCardNum, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downrightSlotInit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downrightSlotPrepareCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downrightSlotSendCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downrightSlotGetCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downrightSlotRecoverCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downrightSlotSetCardNum, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotInit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotSetStatus, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotBackCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotRecoverCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotInit, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotSetStatus, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotBackCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotRecoverCard, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_robotModeBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_sentryboxModeBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upleftSlotInit_Burn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upleftSlotOpenReader, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upleftSlotSetCardNum_Burn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upleftSlotStartBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upleftSlotStopBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_uprightSlotInit_Burn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_uprightSlotOpenReader, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_uprightSlotSetCardNum_Burn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_uprightSlotStartBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_uprightSlotStopBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downleftSlotInit_Burn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downleftSlotOpenReader, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downleftSlotSetCardNum_Burn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downleftSlotStartBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downleftSlotStopBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downrightSlotInit_Burn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downrightSlotOpenReader, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downrightSlotSetCardNum_Burn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downrightSlotStartBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downrightSlotStopBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotInit_Burn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotOpenReader, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotInitReader, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotStartBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_upSlotStopBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotInit_Burn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotOpenReader, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotInitReader, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotStartBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));
    connect(ui->button_downSlotStopBurn, SIGNAL(clicked()), m_pSignalMapperRobot , SLOT(map()));

    m_pSignalMapperNode->setMapping(ui->button_trigger1_A, 0x3011A);
    m_pSignalMapperNode->setMapping(ui->button_trigger2_A, 0x3012A);
    m_pSignalMapperNode->setMapping(ui->button_trigger3_A, 0x3013A);
    m_pSignalMapperNode->setMapping(ui->button_trigger4_A, 0x3014A);
    m_pSignalMapperNode->setMapping(ui->button_trigger5_A, 0x3015A);
    m_pSignalMapperNode->setMapping(ui->button_trigger6_A, 0x3016A);
    m_pSignalMapperNode->setMapping(ui->button_trigger1_B, 0x3011B);
    m_pSignalMapperNode->setMapping(ui->button_trigger2_B, 0x3012B);
    m_pSignalMapperNode->setMapping(ui->button_trigger3_B, 0x3013B);
    m_pSignalMapperNode->setMapping(ui->button_trigger4_B, 0x3014B);
    m_pSignalMapperNode->setMapping(ui->button_trigger5_B, 0x3015B);
    m_pSignalMapperNode->setMapping(ui->button_trigger6_B, 0x3016B);
    m_pSignalMapperNode->setMapping(ui->button_relay1Open_A, 0x30211A);
    m_pSignalMapperNode->setMapping(ui->button_relay1Close_A, 0x30210A);
    m_pSignalMapperNode->setMapping(ui->button_relay2Open_A, 0x30221A);
    m_pSignalMapperNode->setMapping(ui->button_relay2Close_A, 0x30220A);
    m_pSignalMapperNode->setMapping(ui->button_relay3Open_A, 0x30231A);
    m_pSignalMapperNode->setMapping(ui->button_relay3Close_A, 0x30230A);
    m_pSignalMapperNode->setMapping(ui->button_relay4Open_A, 0x30241A);
    m_pSignalMapperNode->setMapping(ui->button_relay4Close_A, 0x30240A);
    m_pSignalMapperNode->setMapping(ui->button_relay5Open_A, 0x30251A);
    m_pSignalMapperNode->setMapping(ui->button_relay5Close_A, 0x30250A);
    m_pSignalMapperNode->setMapping(ui->button_relay6Open_A, 0x30261A);
    m_pSignalMapperNode->setMapping(ui->button_relay6Close_A, 0x30260A);
    m_pSignalMapperNode->setMapping(ui->button_relay7Open_A, 0x30271A);
    m_pSignalMapperNode->setMapping(ui->button_relay7Close_A, 0x30270A);
    m_pSignalMapperNode->setMapping(ui->button_relay8Open_A, 0x30281A);
    m_pSignalMapperNode->setMapping(ui->button_relay8Close_A, 0x30280A);
    m_pSignalMapperNode->setMapping(ui->button_relay1Open_B, 0x30211B);
    m_pSignalMapperNode->setMapping(ui->button_relay1Close_B, 0x30210B);
    m_pSignalMapperNode->setMapping(ui->button_relay2Open_B, 0x30221B);
    m_pSignalMapperNode->setMapping(ui->button_relay2Close_B, 0x30220B);
    m_pSignalMapperNode->setMapping(ui->button_relay3Open_B, 0x30231B);
    m_pSignalMapperNode->setMapping(ui->button_relay3Close_B, 0x30230B);
    m_pSignalMapperNode->setMapping(ui->button_relay4Open_B, 0x30241B);
    m_pSignalMapperNode->setMapping(ui->button_relay4Close_B, 0x30240B);
    m_pSignalMapperNode->setMapping(ui->button_relay5Open_B, 0x30251B);
    m_pSignalMapperNode->setMapping(ui->button_relay5Close_B, 0x30250B);
    m_pSignalMapperNode->setMapping(ui->button_relay6Open_B, 0x30261B);
    m_pSignalMapperNode->setMapping(ui->button_relay6Close_B, 0x30260B);
    m_pSignalMapperNode->setMapping(ui->button_relay7Open_B, 0x30271B);
    m_pSignalMapperNode->setMapping(ui->button_relay7Close_B, 0x30270B);
    m_pSignalMapperNode->setMapping(ui->button_relay8Open_B, 0x30281B);
    m_pSignalMapperNode->setMapping(ui->button_relay8Close_B, 0x30280B);
    m_pSignalMapperNode->setMapping(ui->button_displayColor_Red_A, 0x3031A);
    m_pSignalMapperNode->setMapping(ui->button_displayColor_Green_A, 0x3032A);
    m_pSignalMapperNode->setMapping(ui->button_displayColor_Yellow_A, 0x3033A);
    m_pSignalMapperNode->setMapping(ui->button_displayColor_Blue_A, 0x3034A);
    m_pSignalMapperNode->setMapping(ui->button_displayColor_Red_B, 0x3031B);
    m_pSignalMapperNode->setMapping(ui->button_displayColor_Green_B, 0x3032B);
    m_pSignalMapperNode->setMapping(ui->button_displayColor_Yellow_B, 0x3033B);
    m_pSignalMapperNode->setMapping(ui->button_displayColor_Blue_B, 0x3034B);
    m_pSignalMapperNode->setMapping(ui->button_volumeTest_A, 0x304A);
    m_pSignalMapperNode->setMapping(ui->button_volumeTest_B, 0x304B);
    m_pSignalMapperNode->setMapping(ui->button_radarTest_A, 0x305A);
    m_pSignalMapperNode->setMapping(ui->button_radarTest_B, 0x305B);
    m_pSignalMapperNode->setMapping(ui->button_railingUp_A, 0x3061A);
    m_pSignalMapperNode->setMapping(ui->button_railingDown_A, 0x3060A);
    m_pSignalMapperNode->setMapping(ui->button_railingUp_B, 0x3061B);
    m_pSignalMapperNode->setMapping(ui->button_railingDown_B, 0x3060B);
    m_pSignalMapperNode->setMapping(ui->button_frontLightOpen_A, 0x30701A);
    m_pSignalMapperNode->setMapping(ui->button_frontLightClose_A, 0x30700A);
    m_pSignalMapperNode->setMapping(ui->button_besideLightOpen_A, 0x30711A);
    m_pSignalMapperNode->setMapping(ui->button_besideLightClose_A, 0x30710A);
    m_pSignalMapperNode->setMapping(ui->button_backLightOpen_A, 0x30721A);
    m_pSignalMapperNode->setMapping(ui->button_backLightClose_A, 0x30720A);
    m_pSignalMapperNode->setMapping(ui->button_frontLightOpen_B, 0x30701B);
    m_pSignalMapperNode->setMapping(ui->button_frontLightClose_B, 0x30700B);
    m_pSignalMapperNode->setMapping(ui->button_besideLightOpen_B, 0x30711B);
    m_pSignalMapperNode->setMapping(ui->button_besideLightClose_B, 0x30710B);
    m_pSignalMapperNode->setMapping(ui->button_backLightOpen_B, 0x30721B);
    m_pSignalMapperNode->setMapping(ui->button_backLightClose_B, 0x30720B);
    m_pSignalMapperRobot->setMapping(ui->button_displayColor_Red_Entry, 0x4011A);
    m_pSignalMapperRobot->setMapping(ui->button_displayColor_Green_Entry, 0x4012A);
    m_pSignalMapperRobot->setMapping(ui->button_displayColor_Yellow_Entry, 0x4013A);
    m_pSignalMapperRobot->setMapping(ui->button_displayColor_Blue_Entry, 0x4014A);
    m_pSignalMapperRobot->setMapping(ui->button_displayColor_Red_Exit, 0x4011D);
    m_pSignalMapperRobot->setMapping(ui->button_displayColor_Green_Exit, 0x4012D);
    m_pSignalMapperRobot->setMapping(ui->button_displayColor_Yellow_Exit, 0x4013D);
    m_pSignalMapperRobot->setMapping(ui->button_displayColor_Blue_Exit, 0x4014D);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight1Open_Entry, 0x40211A);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight1Close_Entry, 0x40210A);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight2Open_Entry, 0x40221A);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight2Close_Entry, 0x40220A);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight3Open_Entry, 0x40231A);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight3Close_Entry, 0x40230A);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight1Open_Exit, 0x40211D);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight1Close_Exit, 0x40210D);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight2Open_Exit, 0x40221D);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight2Close_Exit, 0x40220D);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight3Open_Exit, 0x40231D);
    m_pSignalMapperRobot->setMapping(ui->button_ctrlLight3Close_Exit, 0x40230D);
    m_pSignalMapperRobot->setMapping(ui->button_to4GNet_Entry, 0x4031A);
    m_pSignalMapperRobot->setMapping(ui->button_toLineNet_Entry, 0x4030A);
    m_pSignalMapperRobot->setMapping(ui->button_to4GNet_Exit, 0x4031D);
    m_pSignalMapperRobot->setMapping(ui->button_toLineNet_Exit, 0x4030D);
    m_pSignalMapperRobot->setMapping(ui->button_audioTest_Entry, 0x4040A);
    m_pSignalMapperRobot->setMapping(ui->button_record_Entry, 0x4041A);
    m_pSignalMapperRobot->setMapping(ui->button_playRecord_Entry, 0x4042A);
    m_pSignalMapperRobot->setMapping(ui->button_audioTest_Exit, 0x4040D);
    m_pSignalMapperRobot->setMapping(ui->button_record_Exit, 0x4041D);
    m_pSignalMapperRobot->setMapping(ui->button_playRecord_Exit, 0x4042D);
    m_pSignalMapperRobot->setMapping(ui->button_openUpCamera_Entry, 0x40511A);
    m_pSignalMapperRobot->setMapping(ui->button_closeUpCamera_Entry, 0x40510A);
    m_pSignalMapperRobot->setMapping(ui->button_openDownCamera_Entry, 0x40501A);
    m_pSignalMapperRobot->setMapping(ui->button_closeDownCamera_Entry, 0x40500A);
    m_pSignalMapperRobot->setMapping(ui->button_openUpCamera_Exit, 0x40511D);
    m_pSignalMapperRobot->setMapping(ui->button_closeUpCamera_Exit, 0x40510D);
    m_pSignalMapperRobot->setMapping(ui->button_openDownCamera_Exit, 0x40501D);
    m_pSignalMapperRobot->setMapping(ui->button_closeDownCamera_Exit, 0x40500D);
    m_pSignalMapperRobot->setMapping(ui->button_telescopicArmInit_Entry, 0x4060A);
    m_pSignalMapperRobot->setMapping(ui->button_telescopicArmTake10_Entry, 0x4061A);
    m_pSignalMapperRobot->setMapping(ui->button_telescopicArmTake20_Entry, 0x4062A);
    m_pSignalMapperRobot->setMapping(ui->button_telescopicArmBack_Entry, 0x4063A);
    m_pSignalMapperRobot->setMapping(ui->button_telescopicArmInit_Exit, 0x4060D);
    m_pSignalMapperRobot->setMapping(ui->button_telescopicArmTake10_Exit, 0x4061D);
    m_pSignalMapperRobot->setMapping(ui->button_telescopicArmTake20_Exit, 0x4062D);
    m_pSignalMapperRobot->setMapping(ui->button_telescopicArmBack_Exit, 0x4063D);
    m_pSignalMapperRobot->setMapping(ui->button_takeETCAntenna_Entry, 0x40701A);
    m_pSignalMapperRobot->setMapping(ui->button_takeCPCAntenna_Entry, 0x40700A);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotWriteCard_Entry, 0x40714A);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotReadCard_Entry, 0x40713A);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotStop_Entry, 0x40715A);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotWriteCard_Entry, 0x40704A);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotReadCard_Entry, 0x40703A);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotStop_Entry, 0x40705A);
    m_pSignalMapperRobot->setMapping(ui->button_takeETCAntenna_Exit, 0x40701D);
    m_pSignalMapperRobot->setMapping(ui->button_takeCPCAntenna_Exit, 0x40700D);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotWriteCard_Exit, 0x40714D);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotReadCard_Exit, 0x40713D);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotStop_Exit, 0x40715D);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotWriteCard_Exit, 0x40704D);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotReadCard_Exit, 0x40703D);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotStop_Exit, 0x40705D);
    m_pSignalMapperRobot->setMapping(ui->button_robotModeCardMachine, 0x40800);
    m_pSignalMapperRobot->setMapping(ui->button_sentryboxModeCardMahine, 0x40810);
    m_pSignalMapperRobot->setMapping(ui->button_upleftSlotInit, 0x40820);
    m_pSignalMapperRobot->setMapping(ui->button_upleftSlotPrepareCard, 0x40821);
    m_pSignalMapperRobot->setMapping(ui->button_upleftSlotSendCard, 0x40822);
    m_pSignalMapperRobot->setMapping(ui->button_upleftSlotGetCard, 0x40823);
    m_pSignalMapperRobot->setMapping(ui->button_upleftSlotRecoverCard, 0x40824);
    m_pSignalMapperRobot->setMapping(ui->button_upleftSlotSetCardNum, 0x40825);
    m_pSignalMapperRobot->setMapping(ui->button_uprightSlotInit, 0x40830);
    m_pSignalMapperRobot->setMapping(ui->button_uprightSlotPrepareCard, 0x40831);
    m_pSignalMapperRobot->setMapping(ui->button_uprightSlotSendCard, 0x40832);
    m_pSignalMapperRobot->setMapping(ui->button_uprightSlotGetCard, 0x40833);
    m_pSignalMapperRobot->setMapping(ui->button_uprightSlotRecoverCard, 0x40834);
    m_pSignalMapperRobot->setMapping(ui->button_uprightSlotSetCardNum, 0x40835);
    m_pSignalMapperRobot->setMapping(ui->button_downleftSlotInit, 0x40840);
    m_pSignalMapperRobot->setMapping(ui->button_downleftSlotPrepareCard, 0x40841);
    m_pSignalMapperRobot->setMapping(ui->button_downleftSlotSendCard, 0x40842);
    m_pSignalMapperRobot->setMapping(ui->button_downleftSlotGetCard, 0x40843);
    m_pSignalMapperRobot->setMapping(ui->button_downleftSlotRecoverCard, 0x40844);
    m_pSignalMapperRobot->setMapping(ui->button_downleftSlotSetCardNum, 0x40845);
    m_pSignalMapperRobot->setMapping(ui->button_downrightSlotInit, 0x40850);
    m_pSignalMapperRobot->setMapping(ui->button_downrightSlotPrepareCard, 0x40851);
    m_pSignalMapperRobot->setMapping(ui->button_downrightSlotSendCard, 0x40852);
    m_pSignalMapperRobot->setMapping(ui->button_downrightSlotGetCard, 0x40853);
    m_pSignalMapperRobot->setMapping(ui->button_downrightSlotRecoverCard, 0x40854);
    m_pSignalMapperRobot->setMapping(ui->button_downrightSlotSetCardNum, 0x40855);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotInit, 0x40900);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotSetStatus, 0x40901);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotBackCard, 0x40902);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotRecoverCard, 0x40903);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotInit, 0x40910);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotSetStatus, 0x40911);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotBackCard, 0x40912);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotRecoverCard, 0x40913);
    m_pSignalMapperRobot->setMapping(ui->button_relay1Open_Entry, 0x41111A);
    m_pSignalMapperRobot->setMapping(ui->button_relay1Close_Entry, 0x41110A);
    m_pSignalMapperRobot->setMapping(ui->button_relay2Open_Entry, 0x41121A);
    m_pSignalMapperRobot->setMapping(ui->button_relay2Close_Entry, 0x41120A);
    m_pSignalMapperRobot->setMapping(ui->button_relay3Open_Entry, 0x41131A);
    m_pSignalMapperRobot->setMapping(ui->button_relay3Close_Entry, 0x41130A);
    m_pSignalMapperRobot->setMapping(ui->button_relay4Open_Entry, 0x41141A);
    m_pSignalMapperRobot->setMapping(ui->button_relay4Close_Entry, 0x41140A);
    m_pSignalMapperRobot->setMapping(ui->button_relay1Open_Exit, 0x41111D);
    m_pSignalMapperRobot->setMapping(ui->button_relay1Close_Exit, 0x41110D);
    m_pSignalMapperRobot->setMapping(ui->button_relay2Open_Exit, 0x41121D);
    m_pSignalMapperRobot->setMapping(ui->button_relay2Close_Exit, 0x41120D);
    m_pSignalMapperRobot->setMapping(ui->button_relay3Open_Exit, 0x41131D);
    m_pSignalMapperRobot->setMapping(ui->button_relay3Close_Exit, 0x41130D);
    m_pSignalMapperRobot->setMapping(ui->button_relay4Open_Exit, 0x41141D);
    m_pSignalMapperRobot->setMapping(ui->button_relay4Close_Exit, 0x41140D);
    m_pSignalMapperRobot->setMapping(ui->button_openScaner_Entry, 0x4121A);
    m_pSignalMapperRobot->setMapping(ui->button_closeScaner_Entry, 0x4120A);
    m_pSignalMapperRobot->setMapping(ui->button_openScaner_Exit, 0x4121D);
    m_pSignalMapperRobot->setMapping(ui->button_closeScaner_Exit, 0x4120D);
    m_pSignalMapperRobot->setMapping(ui->button_robotModeBurn, 0x41300);
    m_pSignalMapperRobot->setMapping(ui->button_sentryboxModeBurn, 0x41310);
    m_pSignalMapperRobot->setMapping(ui->button_upleftSlotInit_Burn, 0x41320);
    m_pSignalMapperRobot->setMapping(ui->button_upleftSlotOpenReader, 0x41321);
    m_pSignalMapperRobot->setMapping(ui->button_upleftSlotSetCardNum_Burn, 0x41323);
    m_pSignalMapperRobot->setMapping(ui->button_upleftSlotStartBurn, 0x41324);
    m_pSignalMapperRobot->setMapping(ui->button_upleftSlotStopBurn, 0x41325);
    m_pSignalMapperRobot->setMapping(ui->button_uprightSlotInit_Burn, 0x41330);
    m_pSignalMapperRobot->setMapping(ui->button_uprightSlotOpenReader, 0x41331);
    m_pSignalMapperRobot->setMapping(ui->button_uprightSlotSetCardNum_Burn, 0x41333);
    m_pSignalMapperRobot->setMapping(ui->button_uprightSlotStartBurn, 0x41334);
    m_pSignalMapperRobot->setMapping(ui->button_uprightSlotStopBurn, 0x41335);
    m_pSignalMapperRobot->setMapping(ui->button_downleftSlotInit_Burn, 0x41340);
    m_pSignalMapperRobot->setMapping(ui->button_downleftSlotOpenReader, 0x41341);
    m_pSignalMapperRobot->setMapping(ui->button_downleftSlotSetCardNum_Burn, 0x41343);
    m_pSignalMapperRobot->setMapping(ui->button_downleftSlotStartBurn, 0x41344);
    m_pSignalMapperRobot->setMapping(ui->button_downleftSlotStopBurn, 0x41345);
    m_pSignalMapperRobot->setMapping(ui->button_downrightSlotInit_Burn, 0x41350);
    m_pSignalMapperRobot->setMapping(ui->button_downrightSlotOpenReader, 0x41351);
    m_pSignalMapperRobot->setMapping(ui->button_downrightSlotSetCardNum_Burn, 0x41353);
    m_pSignalMapperRobot->setMapping(ui->button_downrightSlotStartBurn, 0x41354);
    m_pSignalMapperRobot->setMapping(ui->button_downrightSlotStopBurn, 0x41355);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotInit_Burn, 0x41400);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotOpenReader, 0x41401);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotInitReader, 0x41402);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotStartBurn, 0x41403);
    m_pSignalMapperRobot->setMapping(ui->button_upSlotStopBurn, 0x41404);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotInit_Burn, 0x41410);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotOpenReader, 0x41411);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotInitReader, 0x41412);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotStartBurn, 0x41413);
    m_pSignalMapperRobot->setMapping(ui->button_downSlotStopBurn, 0x41414);

    connect(m_pSignalMapperNode, SIGNAL(mapped(int)), this, SLOT(slotNodeTestButton_onClicked(int)));
    connect(m_pSignalMapperRobot, SIGNAL(mapped(int)), this, SLOT(slotRobotTestButton_onClicked(int)));
    m_pWebSocketClient->setHeartMsg(RobotDataParse::getInstance()->generateHeartMsg());
}

void Mainwindow::Create_Simple_JSON(void)
{
    QJsonObject root_Obj;
    root_Obj.insert("nCmd", 3);
    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Compact);
    QString strJson(root_str);
    qDebug() << strJson;

    /* 测试：QString returnVal = QString::number(root_Obj["nCmd"].toInt());
    QByteArray data = returnVal.toUtf8();
    qDebug() << data;
    qDebug() << returnVal;*/
}

void Mainwindow::slotNodeTestButton_onClicked(int nType)
{
    QString strData;
    QStringList strlist;

    if(nType == 0x3031A || nType == 0x3032A || nType == 0x3033A || nType == 0x3034A)
    {
        strlist.append(ui->lineEdit_line1_A->text());
        strlist.append(ui->lineEdit_line2_A->text());
        strlist.append(ui->lineEdit_line3_A->text());
        strlist.append(ui->lineEdit_line4_A->text());
        strlist.append(ui->lineEdit_line5_A->text());
        strlist.append(ui->lineEdit_line6_A->text());
        strlist.append(ui->lineEdit_line7_A->text());
        strlist.append(ui->lineEdit_line8_A->text());
    }
    if(nType == 0x3031B || nType == 0x3032B || nType == 0x3033B || nType == 0x3034B)
    {
        strlist.append(ui->lineEdit_line1_B->text());
        strlist.append(ui->lineEdit_line2_B->text());
        strlist.append(ui->lineEdit_line3_B->text());
        strlist.append(ui->lineEdit_line4_B->text());
        strlist.append(ui->lineEdit_line5_B->text());
        strlist.append(ui->lineEdit_line6_B->text());
        strlist.append(ui->lineEdit_line7_B->text());
        strlist.append(ui->lineEdit_line8_B->text());
    }

    switch(nType)
    {///*
    case 0x3011A:
        m_nTrigger = 0x1A;
        ui->button_trigger1_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(1);
        break;
    case 0x3012A:
        m_nTrigger = 0x2A;
        ui->button_trigger2_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(2);
        break;
    case 0x3013A:
        m_nTrigger = 0x3A;
        ui->button_trigger3_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(3);
        break;
    case 0x3014A:
        m_nTrigger = 0x4A;
        ui->button_trigger4_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(4);
        break;
    case 0x3015A:
        m_nTrigger = 0x5A;
        ui->button_trigger5_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(5);
        break;
    case 0x3016A:
        m_nTrigger = 0x6A;
        ui->button_trigger6_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(6);
        break;
    case 0x3011B:
        m_nTrigger = 0x1B;
        ui->button_trigger1_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(1);
        break;
    case 0x3012B:
        m_nTrigger = 0x2B;
        ui->button_trigger2_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(2);
        break;
    case 0x3013B:
        m_nTrigger = 0x3B;
        ui->button_trigger3_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(3);
        break;
    case 0x3014B:
        m_nTrigger = 0x4B;
        ui->button_trigger4_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(4);
        break;
    case 0x3015B:
        m_nTrigger = 0x5B;
        ui->button_trigger5_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(5);
        break;
    case 0x3016B:
        m_nTrigger = 0x6B;
        ui->button_trigger6_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTriggerTest(6);
        break;
    case 0x30211A:
        m_nRelay = 0x11A;
        ui->button_relay1Open_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(1, 1);
        break;
    case 0x30210A:
        m_nRelay = 0x10A;
        ui->button_relay1Close_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(1, 0);
        break;
    case 0x30221A:
        m_nRelay = 0x21A;
        ui->button_relay2Open_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(2, 1);
        break;
    case 0x30220A:
        m_nRelay = 0x20A;
        ui->button_relay2Close_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(2, 0);
        break;
    case 0x30231A:
        m_nRelay = 0x31A;
        ui->button_relay3Open_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(3, 1);
        break;
    case 0x30230A:
        m_nRelay = 0x30A;
        ui->button_relay3Close_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(3, 0);
        break;
    case 0x30241A:
        m_nRelay = 0x41A;
        ui->button_relay4Open_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(4, 1);
        break;
    case 0x30240A:
        m_nRelay = 0x40A;
        ui->button_relay4Close_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(4, 0);
        break;
    case 0x30251A:
        m_nRelay = 0x51A;
        ui->button_relay5Open_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(5, 1);
        break;
    case 0x30250A:
        m_nRelay = 0x50A;
        ui->button_relay5Close_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(5, 0);
        break;
    case 0x30261A:
        m_nRelay = 0x61A;
        ui->button_relay6Open_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(6, 1);
        break;
    case 0x30260A:
        m_nRelay = 0x60A;
        ui->button_relay6Close_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(6, 0);
        break;
    case 0x30271A:
        m_nRelay = 0x71A;
        ui->button_relay7Open_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(7, 1);
        break;
    case 0x30270A:
        m_nRelay = 0x70A;
        ui->button_relay7Close_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(7, 0);
        break;
    case 0x30281A:
        m_nRelay = 0x81A;
        ui->button_relay8Open_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(8, 1);
        break;
    case 0x30280A:
        m_nRelay = 0x80A;
        ui->button_relay8Close_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(8, 0);
        break;
    case 0x30211B:
        m_nRelay = 0x11B;
        ui->button_relay1Open_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(1, 1);
        break;
    case 0x30210B:
        m_nRelay = 0x10B;
        ui->button_relay1Close_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(1, 0);
        break;
    case 0x30221B:
        m_nRelay = 0x21B;
        ui->button_relay2Open_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(2, 1);
        break;
    case 0x30220B:
        m_nRelay = 0x20B;
        ui->button_relay2Close_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(2, 0);
        break;
    case 0x30231B:
        m_nRelay = 0x31B;
        ui->button_relay3Open_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(3, 1);
        break;
    case 0x30230B:
        m_nRelay = 0x30B;
        ui->button_relay3Close_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(3, 0);
        break;
    case 0x30241B:
        m_nRelay = 0x41B;
        ui->button_relay4Open_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(4, 1);
        break;
    case 0x30240B:
        m_nRelay = 0x40B;
        ui->button_relay4Close_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(4, 0);
        break;
    case 0x30251B:
        m_nRelay = 0x51B;
        ui->button_relay5Open_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(5, 1);
        break;
    case 0x30250B:
        m_nRelay = 0x50B;
        ui->button_relay5Close_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(5, 0);
        break;
    case 0x30261B:
        m_nRelay = 0x61B;
        ui->button_relay6Open_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(6, 1);
        break;
    case 0x30260B:
        m_nRelay = 0x60B;
        ui->button_relay6Close_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(6, 0);
        break;
    case 0x30271B:
        m_nRelay = 0x71B;
        ui->button_relay7Open_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(7, 1);
        break;
    case 0x30270B:
        m_nRelay = 0x70B;
        ui->button_relay7Close_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(7, 0);
        break;
    case 0x30281B:
        m_nRelay = 0x81B;
        ui->button_relay8Open_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(8, 1);
        break;
    case 0x30280B:
        m_nRelay = 0x80B;
        ui->button_relay8Close_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayTest(8, 0);
        break;
    case 0x3031A:
        m_nDisplay = 0x1A;
        ui->button_displayColor_Red_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateDisplayTest(strlist, 1);
        break;
    case 0x3032A:
        m_nDisplay = 0x2A;
        ui->button_displayColor_Green_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateDisplayTest(strlist, 2);
        break;
    case 0x3033A:
        m_nDisplay = 0x3A;
        ui->button_displayColor_Yellow_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateDisplayTest(strlist, 3);
        break;
    case 0x3034A:
        m_nDisplay = 0x4A;
        ui->button_displayColor_Blue_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateDisplayTest(strlist, 4);
        break;
    case 0x3031B:
        m_nDisplay = 0x1B;
        ui->button_displayColor_Red_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateDisplayTest(strlist, 1);
        break;
    case 0x3032B:
        m_nDisplay = 0x2B;
        ui->button_displayColor_Green_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateDisplayTest(strlist, 2);
        break;
    case 0x3033B:
        m_nDisplay = 0x3B;
        ui->button_displayColor_Yellow_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateDisplayTest(strlist, 3);
        break;
    case 0x3034B:
        m_nDisplay = 0x4B;
        ui->button_displayColor_Blue_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateDisplayTest(strlist, 4);
        break;//*/
    case 0x304A:
        m_nVolume = 0xA;
        ui->button_volumeTest_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateVolumeTest(ui->lineEdit_volume_A->text().toInt());
        break;
    case 0x304B:
        m_nVolume = 0xB;
        ui->button_volumeTest_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateVolumeTest(ui->lineEdit_volume_B->text().toInt());
        break;
    case 0x305A:
        m_nRadar = 0xA;
        ui->button_radarTest_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRadarTest(1);
        break;
    case 0x305B:
        m_nRadar = 0xB;
        ui->button_radarTest_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRadarTest(1);
        break;
    case 0x3061A:
        m_nRail = 0x1A;
        ui->button_railingUp_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRailingTest(1);
        break;
    case 0x3060A:
        m_nRail = 0x0A;
        ui->button_railingDown_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRailingTest(0);
        break;
    case 0x3061B:
        m_nRail = 0x1B;
        ui->button_railingUp_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRailingTest(1);
        break;
    case 0x3060B:
        m_nRail = 0x0B;
        ui->button_railingDown_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRailingTest(0);
        break;
    case 0x30701A:
        m_nLight = 0x01A;
        ui->button_frontLightOpen_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(0, 1, ui->lineEdit_frontLightValue_A->text().toInt());
        break;
    case 0x30700A:
        m_nLight = 0x00A;
        ui->button_frontLightClose_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(0, 0, ui->lineEdit_frontLightValue_A->text().toInt());
        break;
    case 0x30711A:
        m_nLight = 0x11A;
        ui->button_besideLightOpen_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(1, 1, ui->lineEdit_besideLightValue_A->text().toInt());
        break;
    case 0x30710A:
        m_nLight = 0x10A;
        ui->button_besideLightClose_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(1, 0, ui->lineEdit_besideLightValue_A->text().toInt());
        break;
    case 0x30721A:
        m_nLight = 0x21A;
        ui->button_backLightOpen_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(2, 1, ui->lineEdit_backLightValue_A->text().toInt());
        break;
    case 0x30720A:
        m_nLight = 0x20A;
        ui->button_backLightClose_A->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(2, 0, ui->lineEdit_backLightValue_A->text().toInt());
        break;
    case 0x30701B:
        m_nLight = 0x01B;
        ui->button_frontLightOpen_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(0, 1, ui->lineEdit_frontLightValue_B->text().toInt());
        break;
    case 0x30700B:
        m_nLight = 0x00B;
        ui->button_frontLightClose_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(0, 0, ui->lineEdit_frontLightValue_B->text().toInt());
        break;
    case 0x30711B:
        m_nLight = 0x11B;
        ui->button_besideLightOpen_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(1, 1, ui->lineEdit_besideLightValue_B->text().toInt());
        break;
    case 0x30710B:
        m_nLight = 0x10B;
        ui->button_besideLightClose_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(1, 0, ui->lineEdit_besideLightValue_B->text().toInt());
        break;
    case 0x30721B:
        m_nLight = 0x21B;
        ui->button_backLightOpen_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(2, 1, ui->lineEdit_backLightValue_B->text().toInt());
        break;
    case 0x30720B:
        m_nLight = 0x20B;
        ui->button_backLightClose_B->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateLightTest(2, 0, ui->lineEdit_backLightValue_B->text().toInt());
        break;
    default:
        break;
    }

    m_pWebSocketClient->send(strData);
}

void Mainwindow::slotRobotTestButton_onClicked(int nType)
{
    QString strData;
    QStringList strlist;

    if(nType == 0x4011A || nType == 0x4012A || nType == 0x4013A || nType == 0x4014A)
    {
        strlist.append(ui->lineEdit_line1_Entry->text());
        strlist.append(ui->lineEdit_line2_Entry->text());
        strlist.append(ui->lineEdit_line3_Entry->text());
    }
    if(nType == 0x4011D || nType == 0x4012D || nType == 0x4013D || nType == 0x4014D)
    {
        strlist.append(ui->lineEdit_line1_Exit->text());
        strlist.append(ui->lineEdit_line2_Exit->text());
        strlist.append(ui->lineEdit_line3_Exit->text());
    }

    switch(nType)
    {///*
    case 0x4011A:
        m_nRobotDisplay = 0x1A;
        ui->button_displayColor_Red_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRobotDisplayTest(strlist, 1);
        break;
    case 0x4012A:
        m_nRobotDisplay = 0x2A;
        ui->button_displayColor_Green_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRobotDisplayTest(strlist, 2);
        break;
    case 0x4013A:
        m_nRobotDisplay = 0x3A;
        ui->button_displayColor_Yellow_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRobotDisplayTest(strlist, 3);
        break;
    case 0x4014A:
        m_nRobotDisplay = 0x4A;
        ui->button_displayColor_Blue_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRobotDisplayTest(strlist, 4);
        break;
    case 0x4011D:
        m_nRobotDisplay = 0x1D;
        ui->button_displayColor_Red_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRobotDisplayTest(strlist, 1);
        break;
    case 0x4012D:
        m_nRobotDisplay = 0x2D;
        ui->button_displayColor_Green_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRobotDisplayTest(strlist, 2);
        break;
    case 0x4013D:
        m_nRobotDisplay = 0x3D;
        ui->button_displayColor_Yellow_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRobotDisplayTest(strlist, 3);
        break;
    case 0x4014D:
        m_nRobotDisplay = 0x4D;
        ui->button_displayColor_Blue_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRobotDisplayTest(strlist, 4);
        break;
    case 0x40211A:
        m_nCtrlLight = 0x11A;
        ui->button_ctrlLight1Open_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(1, 1);
        break;
    case 0x40210A:
        m_nCtrlLight = 0x10A;
        ui->button_ctrlLight1Close_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(1, 0);
        break;
    case 0x40221A:
        m_nCtrlLight = 0x21A;
        ui->button_ctrlLight2Open_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(2, 1);
        break;
    case 0x40220A:
        m_nCtrlLight = 0x20A;
        ui->button_ctrlLight2Close_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(2, 0);
        break;
    case 0x40231A:
        m_nCtrlLight = 0x31A;
        ui->button_ctrlLight3Open_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(3, 1);
        break;
    case 0x40230A:
        m_nCtrlLight = 0x30A;
        ui->button_ctrlLight3Close_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(3, 0);
        break;
    case 0x40211D:
        m_nCtrlLight = 0x11D;
        ui->button_ctrlLight1Open_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(1, 1);
        break;
    case 0x40210D:
        m_nCtrlLight = 0x10D;
        ui->button_ctrlLight1Close_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(1, 0);
        break;
    case 0x40221D:
        m_nCtrlLight = 0x21D;
        ui->button_ctrlLight2Open_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(2, 1);
        break;
    case 0x40220D:
        m_nCtrlLight = 0x20D;
        ui->button_ctrlLight2Close_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(2, 0);
        break;
    case 0x40231D:
        m_nCtrlLight = 0x31D;
        ui->button_ctrlLight3Open_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(3, 1);
        break;
    case 0x40230D:
        m_nCtrlLight = 0x30D;
        ui->button_ctrlLight3Close_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCtrlLightTest(3, 0);
        break;
    case 0x4031A:
        m_nNet = 0x1A;
        ui->button_to4GNet_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateNetTest(1);
        break;
    case 0x4030A:
        m_nNet = 0x0A;
        ui->button_toLineNet_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateNetTest(0);
        break;
    case 0x4031D:
        m_nNet = 0x1D;
        ui->button_to4GNet_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateNetTest(1);
        break;
    case 0x4030D:
        m_nNet = 0x0D;
        ui->button_toLineNet_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateNetTest(0);
        break;
    case 0x4040A:
        m_nAudio = 0x0A;
        ui->button_audioTest_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateAudioTest(ui->lineEdit_audioVolume_Entry->text().toInt(), 0);
        break;
    case 0x4041A:
        m_nAudio = 0x1A;
        ui->button_record_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateAudioTest(ui->lineEdit_audioVolume_Entry->text().toInt(), 1);
        break;
    case 0x4042A:
        m_nAudio = 0x2A;
        ui->button_playRecord_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateAudioTest(ui->lineEdit_audioVolume_Entry->text().toInt(), 2);
        break;
    case 0x4040D:
        m_nAudio = 0x0D;
        ui->button_audioTest_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateAudioTest(ui->lineEdit_audioVolume_Exit->text().toInt(), 0);
        break;
    case 0x4041D:
        m_nAudio = 0x1D;
        ui->button_record_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateAudioTest(ui->lineEdit_audioVolume_Exit->text().toInt(), 1);
        break;
    case 0x4042D:
        m_nAudio = 0x2D;
        ui->button_playRecord_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateAudioTest(ui->lineEdit_audioVolume_Exit->text().toInt(), 2);
        break;
    case 0x40511A:
        m_nCamera = 0x11A;
        ui->button_openUpCamera_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCameraTest(1, 1);
        break;
    case 0x40510A:
        m_nCamera = 0x10A;
        ui->button_closeUpCamera_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCameraTest(1, 0);
        break;
    case 0x40501A:
        m_nCamera = 0x01A;
        ui->button_openDownCamera_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCameraTest(0, 1);
        break;
    case 0x40500A:
        m_nCamera = 0x00A;
        ui->button_closeDownCamera_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCameraTest(0, 0);
        break;
    case 0x40511D:
        m_nCamera = 0x11D;
        ui->button_openUpCamera_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCameraTest(1, 1);
        break;
    case 0x40510D:
        m_nCamera = 0x10D;
        ui->button_closeUpCamera_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCameraTest(1, 0);
        break;
    case 0x40501D:
        m_nCamera = 0x01D;
        ui->button_openDownCamera_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCameraTest(0, 1);
        break;
    case 0x40500D:
        m_nCamera = 0x00D;
        ui->button_closeDownCamera_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateCameraTest(0, 0);
        break;
    case 0x4060A:
        m_nTelescopicArm = 0x0A;
        ui->button_telescopicArmInit_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTelescopicArmTest(0);
        break;
    case 0x4061A:
        m_nTelescopicArm = 0x1A;
        ui->button_telescopicArmTake10_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTelescopicArmTest(1);
        break;
    case 0x4062A:
        m_nTelescopicArm = 0x2A;
        ui->button_telescopicArmTake20_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTelescopicArmTest(2);
        break;
    case 0x4063A:
        m_nTelescopicArm = 0x3A;
        ui->button_telescopicArmBack_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTelescopicArmTest(3);
        break;
    case 0x4060D:
        m_nTelescopicArm = 0x0D;
        ui->button_telescopicArmInit_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTelescopicArmTest(0);
        break;
    case 0x4061D:
        m_nTelescopicArm = 0x1D;
        ui->button_telescopicArmTake10_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTelescopicArmTest(1);
        break;
    case 0x4062D:
        m_nTelescopicArm = 0x2D;
        ui->button_telescopicArmTake20_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTelescopicArmTest(2);
        break;
    case 0x4063D:
        m_nTelescopicArm = 0x3D;
        ui->button_telescopicArmBack_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateTelescopicArmTest(3);
        break;
    case 0x40701A:
        m_nReader= 0x01A;
        ui->button_takeETCAntenna_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(1, 1);
        break;
    case 0x40700A:
        m_nReader= 0x00A;
        ui->button_takeCPCAntenna_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(1, 0);
        break;
    case 0x40714A:
        m_nReader= 0x14A;
        ui->button_upSlotWriteCard_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(1, 4);
        break;
    case 0x40713A:
        m_nReader= 0x13A;
        ui->button_upSlotReadCard_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(1, 3);
        break;
    case 0x40715A:
        m_nReader= 0x15A;
        ui->button_upSlotStop_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(1, 5);
        break;
    case 0x40704A:
        m_nReader= 0x04A;
        ui->button_downSlotWriteCard_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(0, 4);
        break;
    case 0x40703A:
        m_nReader= 0x03A;
        ui->button_downSlotReadCard_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(0, 3);
        break;
    case 0x40705A:
        m_nReader= 0x05A;
        ui->button_downSlotStop_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(0, 5);
        break;
    case 0x40701D:
        m_nReader= 0x01D;
        ui->button_takeETCAntenna_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(1, 1);
        break;
    case 0x40700D:
        m_nReader= 0x00D;
        ui->button_takeCPCAntenna_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(1, 0);
        break;
    case 0x40714D:
        m_nReader= 0x14D;
        ui->button_upSlotWriteCard_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(1, 4);
        break;
    case 0x40713D:
        m_nReader= 0x13D;
        ui->button_upSlotReadCard_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(1, 3);
        break;
    case 0x40704D:
        m_nReader= 0x04D;
        ui->button_downSlotWriteCard_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(0, 4);
        break;
    case 0x40703D:
        m_nReader= 0x03D;
        ui->button_downSlotReadCard_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(0, 3);
        break;
    case 0x40705D:
        m_nReader= 0x05D;
        ui->button_downSlotStop_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(0, 5);
        break;
    case 0x40715D:
        m_nReader= 0x15D;
        ui->button_upSlotStop_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateReaderTest(1, 5);
        break;
    case 0x40800:
        m_nEntryMachine = 00;
        ui->button_robotModeCardMachine->setStyleSheet(PRESSED_STYLE_ENTRY);
        ui->button_sentryboxModeCardMahine->setStyleSheet(ORIGNAL_STYLE_ENTRY);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(0, 0, ui->lineEdit_upleftSlotCardNum->text().toInt());
        break;
    case 0x40810:
        m_nEntryMachine = 10;
        ui->button_sentryboxModeCardMahine->setStyleSheet(PRESSED_STYLE_ENTRY);
        ui->button_robotModeCardMachine->setStyleSheet(ORIGNAL_STYLE_ENTRY);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(1, 0, ui->lineEdit_upleftSlotCardNum->text().toInt());
        break;
    case 0x40820:
        m_nEntryMachine= 20;
        ui->button_upleftSlotInit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(2, 0, ui->lineEdit_upleftSlotCardNum->text().toInt());
        break;
    case 0x40821:
        m_nEntryMachine= 21;
        ui->button_upleftSlotPrepareCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(2, 1, ui->lineEdit_upleftSlotCardNum->text().toInt());
        break;
    case 0x40822:
        m_nEntryMachine= 22;
        ui->button_upleftSlotSendCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(2, 2, ui->lineEdit_upleftSlotCardNum->text().toInt());
        break;
    case 0x40823:
        m_nEntryMachine= 23;
        ui->button_upleftSlotGetCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(2, 3, ui->lineEdit_upleftSlotCardNum->text().toInt());
        break;
    case 0x40824:
        m_nEntryMachine= 24;
        ui->button_upleftSlotRecoverCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(2, 4, ui->lineEdit_upleftSlotCardNum->text().toInt());
        break;
    case 0x40825:
        m_nEntryMachine= 25;
        ui->button_upleftSlotSetCardNum->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(2, 5, ui->lineEdit_upleftSlotCardNum->text().toInt());
        break;
    case 0x40830:
        m_nEntryMachine= 30;
        ui->button_uprightSlotInit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(3, 0, ui->lineEdit_uprightSlotCardNum->text().toInt());
        break;
    case 0x40831:
        m_nEntryMachine= 31;
        ui->button_uprightSlotPrepareCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(3, 1, ui->lineEdit_uprightSlotCardNum->text().toInt());
        break;
    case 0x40832:
        m_nEntryMachine= 32;
        ui->button_uprightSlotSendCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(3, 2, ui->lineEdit_uprightSlotCardNum->text().toInt());
        break;
    case 0x40833:
        m_nEntryMachine= 33;
        ui->button_uprightSlotGetCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(3, 3, ui->lineEdit_uprightSlotCardNum->text().toInt());
        break;
    case 0x40834:
        m_nEntryMachine= 34;
        ui->button_uprightSlotRecoverCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(3, 4, ui->lineEdit_uprightSlotCardNum->text().toInt());
        break;
    case 0x40835:
        m_nEntryMachine= 35;
        ui->button_uprightSlotSetCardNum->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(3, 5, ui->lineEdit_uprightSlotCardNum->text().toInt());
        break;
    case 0x40840:
        m_nEntryMachine= 40;
        ui->button_downleftSlotInit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(4, 0, ui->lineEdit_downleftSlotCardNum->text().toInt());
        break;
    case 0x40841:
        m_nEntryMachine= 41;
        ui->button_downleftSlotPrepareCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(4, 1, ui->lineEdit_downleftSlotCardNum->text().toInt());
        break;
    case 0x40842:
        m_nEntryMachine= 42;
        ui->button_downleftSlotSendCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(4, 2, ui->lineEdit_downleftSlotCardNum->text().toInt());
        break;
    case 0x40843:
        m_nEntryMachine= 43;
        ui->button_downleftSlotGetCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(4, 3, ui->lineEdit_downleftSlotCardNum->text().toInt());
        break;
    case 0x40844:
        m_nEntryMachine= 44;
        ui->button_downleftSlotRecoverCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(4, 4, ui->lineEdit_downleftSlotCardNum->text().toInt());
        break;
    case 0x40845:
        m_nEntryMachine= 45;
        ui->button_downleftSlotSetCardNum->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(4, 5, ui->lineEdit_downleftSlotCardNum->text().toInt());
        break;
    case 0x40850:
        m_nEntryMachine= 50;
        ui->button_downrightSlotInit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(5, 0, ui->lineEdit_downrightSlotCardNum->text().toInt());
        break;
    case 0x40851:
        m_nEntryMachine= 51;
        ui->button_downrightSlotPrepareCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(5, 1, ui->lineEdit_downrightSlotCardNum->text().toInt());
        break;
    case 0x40852:
        m_nEntryMachine= 52;
        ui->button_downrightSlotSendCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(5, 2, ui->lineEdit_downrightSlotCardNum->text().toInt());
        break;
    case 0x40853:
        m_nEntryMachine= 53;
        ui->button_downrightSlotGetCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(5, 3, ui->lineEdit_downrightSlotCardNum->text().toInt());
        break;
    case 0x40854:
        m_nEntryMachine= 54;
        ui->button_downrightSlotRecoverCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(5, 4, ui->lineEdit_downrightSlotCardNum->text().toInt());
        break;
    case 0x40855:
        m_nEntryMachine= 55;
        ui->button_downrightSlotSetCardNum->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryMachineTest(5, 5, ui->lineEdit_downrightSlotCardNum->text().toInt());
        break;
    case 0x40900:
        m_nExitMachine= 00;
        ui->button_upSlotInit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitMachineTest(0, 0);
        break;
    case 0x40901:
        m_nExitMachine= 01;
        ui->button_upSlotSetStatus->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitMachineTest(0, 1);
        break;
    case 0x40902:
        m_nExitMachine= 02;
        ui->button_upSlotBackCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitMachineTest(0, 2);
        break;
    case 0x40903:
        m_nExitMachine= 03;
        ui->button_upSlotRecoverCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitMachineTest(0, 3);
        break;
    case 0x40910:
        m_nExitMachine= 10;
        ui->button_downSlotInit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitMachineTest(1, 0);
        break;
    case 0x40911:
        m_nExitMachine= 11;
        ui->button_downSlotSetStatus->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitMachineTest(1, 1);
        break;
    case 0x40912:
        m_nExitMachine= 12;
        ui->button_downSlotBackCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitMachineTest(1, 2);
        break;
    case 0x40913:
        m_nExitMachine= 13;
        ui->button_downSlotRecoverCard->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitMachineTest(1, 3);
        break;
    case 0x41111A:
        m_nRobotRelay= 0x11A;
        ui->button_relay1Open_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(1, 1);
        break;
    case 0x41110A:
        m_nRobotRelay= 0x10A;
        ui->button_relay1Close_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(1, 0);
        break;
    case 0x41121A:
        m_nRobotRelay= 0x21A;
        ui->button_relay2Open_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(2, 1);
        break;
    case 0x41120A:
        m_nRobotRelay= 0x20A;
        ui->button_relay2Close_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(2, 0);
        break;
    case 0x41131A:
        m_nRobotRelay= 0x31A;
        ui->button_relay3Open_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(3, 1);
        break;
    case 0x41130A:
        m_nRobotRelay= 0x30A;
        ui->button_relay3Close_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(3, 0);
        break;
    case 0x41141A:
        m_nRobotRelay= 0x41A;
        ui->button_relay4Open_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(4, 1);
        break;
    case 0x41140A:
        m_nRobotRelay= 0x40A;
        ui->button_relay4Close_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(4, 0);
        break;
    case 0x41111D:
        m_nRobotRelay= 0x11D;
        ui->button_relay1Open_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(1, 1);
        break;
    case 0x41110D:
        m_nRobotRelay= 0x10D;
        ui->button_relay1Close_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(1, 0);
        break;
    case 0x41121D:
        m_nRobotRelay= 0x21D;
        ui->button_relay2Open_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(2, 1);
        break;
    case 0x41120D:
        m_nRobotRelay= 0x20D;
        ui->button_relay2Close_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(2, 0);
        break;
    case 0x41131D:
        m_nRobotRelay= 0x31D;
        ui->button_relay3Open_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(3, 1);
        break;
    case 0x41130D:
        m_nRobotRelay= 0x30D;
        ui->button_relay3Close_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(3, 0);
        break;
    case 0x41141D:
        m_nRobotRelay= 0x41D;
        ui->button_relay4Open_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(4, 1);
        break;
    case 0x41140D:
        m_nRobotRelay= 0x40D;
        ui->button_relay4Close_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateRelayRobotTest(4, 0);
        break;
    case 0x4121A:
        m_nScaner= 0x1A;
        ui->button_openScaner_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateScanerTest(1);
        break;
    case 0x4120A:
        m_nScaner= 0x0A;
        ui->button_closeScaner_Entry->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateScanerTest(0);
        break;
    case 0x4121D:
        m_nScaner= 0x1D;
        ui->button_openScaner_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateScanerTest(1);
        break;
    case 0x4120D:
        m_nScaner= 0x0D;
        ui->button_closeScaner_Exit->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateScanerTest(0);
        break;
    case 0x41300:
        m_nEntryBurn = 00;
        ui->button_robotModeBurn->setStyleSheet(PRESSED_STYLE_ENTRY);
        ui->button_sentryboxModeBurn->setStyleSheet(ORIGNAL_STYLE_ENTRY);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(0, 0, ui->lineEdit_upleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41310:
        m_nEntryBurn = 10;
        ui->button_sentryboxModeBurn->setStyleSheet(PRESSED_STYLE_ENTRY);
        ui->button_robotModeBurn->setStyleSheet(ORIGNAL_STYLE_ENTRY);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(1, 0, ui->lineEdit_upleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41320:
        m_nEntryBurn = 20;
        ui->button_upleftSlotInit_Burn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(2, 0, ui->lineEdit_upleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41321:
        m_nEntryBurn = 21;
        ui->button_upleftSlotOpenReader->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(2, 1, ui->lineEdit_upleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41323:
        m_nEntryBurn = 23;
        ui->button_upleftSlotSetCardNum_Burn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(2, 3, ui->lineEdit_upleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41324:
        m_nEntryBurn = 24;
        ui->button_upleftSlotStartBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(2, 4, ui->lineEdit_upleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41325:
        m_nEntryBurn = 25;
        ui->button_upleftSlotStopBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(2, 5, ui->lineEdit_upleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41330:
        m_nEntryBurn = 30;
        ui->button_uprightSlotInit_Burn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(3, 0, ui->lineEdit_uprightSlotCardNum_Burn->text().toInt());
        break;
    case 0x41331:
        m_nEntryBurn = 31;
        ui->button_uprightSlotOpenReader->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(3, 1, ui->lineEdit_uprightSlotCardNum_Burn->text().toInt());
        break;
    case 0x41333:
        m_nEntryBurn = 33;
        ui->button_uprightSlotSetCardNum_Burn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(3, 3, ui->lineEdit_uprightSlotCardNum_Burn->text().toInt());
        break;
    case 0x41334:
        m_nEntryBurn = 34;
        ui->button_uprightSlotStartBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(3, 4, ui->lineEdit_uprightSlotCardNum_Burn->text().toInt());
        break;
    case 0x41335:
        m_nEntryBurn = 35;
        ui->button_uprightSlotStopBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(3, 5, ui->lineEdit_downleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41340:
        m_nEntryBurn = 40;
        ui->button_downleftSlotInit_Burn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(4, 0, ui->lineEdit_downleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41341:
        m_nEntryBurn = 41;
        ui->button_downleftSlotOpenReader->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(4, 1, ui->lineEdit_downleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41343:
        m_nEntryBurn = 43;
        ui->button_downleftSlotSetCardNum_Burn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(4, 3, ui->lineEdit_downleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41344:
        m_nEntryBurn = 44;
        ui->button_downleftSlotStartBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(4, 4, ui->lineEdit_downleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41345:
        m_nEntryBurn = 45;
        ui->button_downleftSlotStopBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(4, 5, ui->lineEdit_downleftSlotCardNum_Burn->text().toInt());
        break;
    case 0x41350:
        m_nEntryBurn = 50;
        ui->button_downrightSlotInit_Burn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(5, 0, ui->lineEdit_downrightSlotCardNum_Burn->text().toInt());
        break;
    case 0x41351:
        m_nEntryBurn = 51;
        ui->button_downrightSlotOpenReader->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(5, 1, ui->lineEdit_downrightSlotCardNum_Burn->text().toInt());
        break;
    case 0x41353:
        m_nEntryBurn = 53;
        ui->button_downrightSlotSetCardNum_Burn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(5, 3, ui->lineEdit_downrightSlotCardNum_Burn->text().toInt());
        break;
    case 0x41354:
        m_nEntryBurn = 54;
        ui->button_downrightSlotStartBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(5, 4, ui->lineEdit_downrightSlotCardNum_Burn->text().toInt());
        break;
    case 0x41355:
        m_nEntryBurn = 55;
        ui->button_downrightSlotStopBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateEntryBurnTest(5, 5, ui->lineEdit_downrightSlotCardNum_Burn->text().toInt());
        break;
    case 0x41400:
        m_nExitBurn = 00;
        ui->button_upSlotInit_Burn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitBurnTest(0, 0);
        break;
    case 0x41401:
        m_nExitBurn = 01;
        ui->button_upSlotOpenReader->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitBurnTest(0, 1);
        break;
    case 0x41402:
        m_nExitBurn = 02;
        ui->button_upSlotInitReader->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitBurnTest(0, 2);
        break;
    case 0x41403:
        m_nExitBurn = 03;
        ui->button_upSlotStartBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitBurnTest(0, 3);
        break;
    case 0x41404:
        m_nExitBurn = 04;
        ui->button_upSlotStopBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitBurnTest(0, 4);
        break;
    case 0x41410:
        m_nExitBurn = 10;
        ui->button_downSlotInit_Burn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitBurnTest(1, 0);
        break;
    case 0x41411:
        m_nExitBurn = 11;
        ui->button_downSlotOpenReader->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitBurnTest(1, 1);
        break;
    case 0x41412:
        m_nExitBurn = 12;
        ui->button_downSlotInitReader->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitBurnTest(1, 2);
        break;
    case 0x41413:
        m_nExitBurn = 13;
        ui->button_downSlotStartBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitBurnTest(1, 3);
        break;
    case 0x41414:
        m_nExitBurn = 14;
        ui->button_downSlotStopBurn->setStyleSheet(PRESSED_STYLE);
        strData = RobotDataParse::getInstance()->generateExitBurnTest(1, 4);
        break;
    default:
        break;
    }

    m_pWebSocketClient->send(strData);
}

void Mainwindow::slotConnect_onClicked()
{
    m_pWebSocketClient->setIp(ui->lineEdit_ip->text());
    m_pWebSocketClient->setPort(ui->lineEdit_port->text().toInt());
    m_pWebSocketClient->connectSvr();
}

void Mainwindow::slotConnectStatus(int nStatus)
{
    if(-1 == nStatus)
        ui->label_connectStatus->setText("���ӶϿ�");
    else if(0 == nStatus)
        ui->label_connectStatus->setText("δ����");
    else if(1 == nStatus)
        ui->label_connectStatus->setText("������");
}

void Mainwindow::slotSetDevUniqueId_onClicked()
{
    ui->button_setDevUniqueId->setStyleSheet(PRESSED_STYLE);

    QString strData = RobotDataParse::getInstance()->generateSetDevUniqueId(ui->lineEdit_devUniqueId->text());
    m_pWebSocketClient->send(strData);
}

void Mainwindow::slotSetNICIp_onClicked()
{
    QMessageBox message(QMessageBox::NoIcon,  "��ʾ",  "ȷ��Ҫ�޸�������������", QMessageBox::Yes | QMessageBox::No, NULL);

    if(message.exec() == QMessageBox::No)
        return;

    ui->button_setNICIp->setStyleSheet(PRESSED_STYLE);

    QString strData = RobotDataParse::getInstance()->generateSetNICIp(ui->lineEdit_ip1->text(), ui->lineEdit_mask1->text(), ui->lineEdit_ip2->text(), ui->lineEdit_mask2->text());
    m_pWebSocketClient->send(strData);
}

void Mainwindow::slotSendHeart_onClicked()
{
    m_pWebSocketClient->slotSendHeartMsg();
}

void Mainwindow::slotNodeATest_onClicked()
{
    ui->stackedWidget_Main->setCurrentIndex(0);
    ui->stackedWidget_Node->setCurrentIndex(0);
    ui->button_NodeBTest->setStyleSheet("");
    ui->button_RobotEntryTest->setStyleSheet("");
    ui->button_RobotExitTest->setStyleSheet("");
    ui->button_NodeATest->setStyleSheet("QPushButton{\n	border-style:solid;\n	border-width:1px;\n	color:rgb(20, 0, 170);\n	background-color: rgba(245, 245, 245, 30);\n	border-color:rgb(20, 0, 255,120);\n}\n\nQPushButton:hover{\n	color:rgb(29, 0, 248);\n	background-color: rgba(240, 245, 240, 100);\n	border-color: rgb(20, 3, 255);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(20,0,170,150);\n	background-color: rgba(245, 245, 245, 30);\n	border-color: rgba(20, 3, 130);\n}");
}

void Mainwindow::slotNodeBTest_onClicked()
{
    ui->stackedWidget_Main->setCurrentIndex(0);
    ui->stackedWidget_Node->setCurrentIndex(1);
    ui->button_NodeATest->setStyleSheet("");
    ui->button_RobotEntryTest->setStyleSheet("");
    ui->button_RobotExitTest->setStyleSheet("");
    ui->button_NodeBTest->setStyleSheet("QPushButton{\n	border-style:solid;\n	border-width:1px;\n	color:rgb(20, 0, 170);\n	background-color: rgba(245, 245, 245, 30);\n	border-color:rgb(20, 0, 255,120);\n}\n\nQPushButton:hover{\n	color:rgb(29, 0, 248);\n	background-color: rgba(240, 245, 240, 100);\n	border-color: rgb(20, 3, 255);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(20,0,170,150);\n	background-color: rgba(245, 245, 245, 30);\n	border-color: rgba(20, 3, 130);\n}");
}

void Mainwindow::slotRobotEntryTest_onClicked()
{
    ui->stackedWidget_Main->setCurrentIndex(1);
    ui->stackedWidget_Robot->setCurrentIndex(0);
    ui->button_NodeATest->setStyleSheet("");
    ui->button_NodeBTest->setStyleSheet("");
    ui->button_RobotExitTest->setStyleSheet("");
    ui->button_RobotEntryTest->setStyleSheet("QPushButton{\n	border-style:solid;\n	border-width:1px;\n	color:rgb(20, 0, 170);\n	background-color: rgba(245, 245, 245, 30);\n	border-color:rgb(20, 0, 255,120);\n}\n\nQPushButton:hover{\n	color:rgb(29, 0, 248);\n	background-color: rgba(240, 245, 240, 100);\n	border-color: rgb(20, 3, 255);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(20,0,170,150);\n	background-color: rgba(245, 245, 245, 30);\n	border-color: rgba(20, 3, 130);\n}");
}

void Mainwindow::slotRobotExitTest_onClicked()
{
    ui->stackedWidget_Main->setCurrentIndex(1);
    ui->stackedWidget_Robot->setCurrentIndex(1);
    ui->button_NodeATest->setStyleSheet("");
    ui->button_NodeBTest->setStyleSheet("");
    ui->button_RobotEntryTest->setStyleSheet("");
    ui->button_RobotExitTest->setStyleSheet("QPushButton{\n	border-style:solid;\n	border-width:1px;\n	color:rgb(20, 0, 170);\n	background-color: rgba(245, 245, 245, 30);\n	border-color:rgb(20, 0, 255,120);\n}\n\nQPushButton:hover{\n	color:rgb(29, 0, 248);\n	background-color: rgba(240, 245, 240, 100);\n	border-color: rgb(20, 3, 255);\n}\n\nQPushButton:pressed{\n	border-style:inset;\n	color:rgba(20,0,170,150);\n	background-color: rgba(245, 245, 245, 30);\n	border-color: rgba(20, 3, 130);\n}");
}

void Mainwindow::slotFrontCameraA_onClicked()
{

}

void Mainwindow::slotBackCamera_onClicked()
{

}

void Mainwindow::slotBesideCamera_onClicked()
{

}

void Mainwindow::slotFrontCameraB_onClicked()
{

}

bool Mainwindow::slotHandleJsonData(QString strData)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(strData.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();

    int nCmd = jsonObj["cmd"].toInt();
    int nExecuteCode = jsonObj["executeCode"].toInt();

    QString strStyle = (0 == nExecuteCode? WORK_STYLE : ERROR_STYLE);

    if((408 == nCmd && (00 == m_nEntryMachine || 10 == m_nEntryMachine))|| (413 == nCmd && (00 == m_nEntryBurn || 10 == m_nEntryBurn)))
        strStyle = (0 == nExecuteCode? WORK_STYLE_STYLE_ENTRY : ERROR_STYLE_ENTRY);

    ui->textBrowser_recv->append(strData);

    QString Cmd_str = QString::number(jsonObj["nCmd"].toInt());
    QByteArray Cmd_arr = Cmd_str.toUtf8();

    switch (nCmd)
    {
    case 201:
        if(0 == nExecuteCode)
            ui->label_heart->setStyleSheet("QLabel\n{\n	border-radius:10;\n	border:1px solid;\n	border-color:rgb(255, 255, 255);\n	background-color:rgb(0, 170, 0)\n}");
        else
            ui->label_heart->setStyleSheet("QLabel\n{\n	border-radius:10;\n	border:1px solid;\n	border-color:rgb(255,255,255);\n	background-color:rgb(255, 0, 0)\n}");

        sleep(1000);
        ui->label_heart->setStyleSheet("QLabel\n{\n	border-radius:10;\n	border:1px solid;\n	border-color:rgb(70,70,70);\n	background-color:rgb(255, 255, 255)\n}");
        break;
    case 203:
        ui->button_setDevUniqueId->setStyleSheet(strStyle);
        break;
    case 204:
        ui->button_setNICIp->setStyleSheet(strStyle);
        break;
    case 205:
        ui->textBrowser_log->append("resultMsg: " + jsonObj["resultMsg"].toString());
        ui->textBrowser_log->append("nCmd: " + QString::number(jsonObj["nCmd"].toInt()));
        break;
    case 301:
        if(0x1A == m_nTrigger)
            ui->button_trigger1_A->setStyleSheet(strStyle);
        else if(0x2A == m_nTrigger)
            ui->button_trigger2_A->setStyleSheet(strStyle);
        else if(0x3A == m_nTrigger)
            ui->button_trigger3_A->setStyleSheet(strStyle);
        else if(0x4A == m_nTrigger)
            ui->button_trigger4_A->setStyleSheet(strStyle);
        else if(0x5A == m_nTrigger)
            ui->button_trigger5_A->setStyleSheet(strStyle);
        else if(0x6A == m_nTrigger)
            ui->button_trigger6_A->setStyleSheet(strStyle);
        else if(0x1B == m_nTrigger)
            ui->button_trigger1_B->setStyleSheet(strStyle);
        else if(0x2B == m_nTrigger)
            ui->button_trigger2_B->setStyleSheet(strStyle);
        else if(0x3B == m_nTrigger)
            ui->button_trigger3_B->setStyleSheet(strStyle);
        else if(0x4B == m_nTrigger)
            ui->button_trigger4_B->setStyleSheet(strStyle);
        else if(0x5B == m_nTrigger)
            ui->button_trigger5_B->setStyleSheet(strStyle);
        else if(0x6B == m_nTrigger)
            ui->button_trigger6_B->setStyleSheet(strStyle);
        break;
    case 302:
        if(0x11A ==m_nRelay)
            ui->button_relay1Open_A->setStyleSheet(strStyle);
        else if(0x10A ==m_nRelay)
            ui->button_relay1Close_A->setStyleSheet(strStyle);
        else if(0x21A ==m_nRelay)
            ui->button_relay2Open_A->setStyleSheet(strStyle);
        else if(0x20A ==m_nRelay)
            ui->button_relay2Close_A->setStyleSheet(strStyle);
        else if(0x31A ==m_nRelay)
            ui->button_relay3Open_A->setStyleSheet(strStyle);
        else if(0x30A ==m_nRelay)
            ui->button_relay3Close_A->setStyleSheet(strStyle);
        else if(0x41A ==m_nRelay)
            ui->button_relay4Open_A->setStyleSheet(strStyle);
        else if(0x40A ==m_nRelay)
            ui->button_relay4Close_A->setStyleSheet(strStyle);
        else if(0x51A ==m_nRelay)
            ui->button_relay5Open_A->setStyleSheet(strStyle);
        else if(0x50A ==m_nRelay)
            ui->button_relay5Close_A->setStyleSheet(strStyle);
        else if(0x61A ==m_nRelay)
            ui->button_relay6Open_A->setStyleSheet(strStyle);
        else if(0x60A ==m_nRelay)
            ui->button_relay6Close_A->setStyleSheet(strStyle);
        else if(0x71A ==m_nRelay)
            ui->button_relay7Open_A->setStyleSheet(strStyle);
        else if(0x70A ==m_nRelay)
            ui->button_relay7Close_A->setStyleSheet(strStyle);
        else if(0x81A ==m_nRelay)
            ui->button_relay8Open_A->setStyleSheet(strStyle);
        else if(0x80A ==m_nRelay)
            ui->button_relay8Close_A->setStyleSheet(strStyle);
        else if(0x11B ==m_nRelay)
            ui->button_relay1Open_B->setStyleSheet(strStyle);
        else if(0x10B ==m_nRelay)
            ui->button_relay1Close_B->setStyleSheet(strStyle);
        else if(0x21B ==m_nRelay)
            ui->button_relay2Open_B->setStyleSheet(strStyle);
        else if(0x20B ==m_nRelay)
            ui->button_relay2Close_B->setStyleSheet(strStyle);
        else if(0x31B ==m_nRelay)
            ui->button_relay3Open_B->setStyleSheet(strStyle);
        else if(0x30B ==m_nRelay)
            ui->button_relay3Close_B->setStyleSheet(strStyle);
        else if(0x41B ==m_nRelay)
            ui->button_relay4Open_B->setStyleSheet(strStyle);
        else if(0x40B ==m_nRelay)
            ui->button_relay4Close_B->setStyleSheet(strStyle);
        else if(0x51B ==m_nRelay)
            ui->button_relay5Open_B->setStyleSheet(strStyle);
        else if(0x50B ==m_nRelay)
            ui->button_relay5Close_B->setStyleSheet(strStyle);
        else if(0x61B ==m_nRelay)
            ui->button_relay6Open_B->setStyleSheet(strStyle);
        else if(0x60B ==m_nRelay)
            ui->button_relay6Close_B->setStyleSheet(strStyle);
        else if(0x71B ==m_nRelay)
            ui->button_relay7Open_B->setStyleSheet(strStyle);
        else if(0x70B ==m_nRelay)
            ui->button_relay7Close_B->setStyleSheet(strStyle);
        else if(0x81B ==m_nRelay)
            ui->button_relay8Open_B->setStyleSheet(strStyle);
        else if(0x80B ==m_nRelay)
            ui->button_relay8Close_B->setStyleSheet(strStyle);
        break;
    case 303:
        if(0x1A == m_nDisplay)
            ui->button_displayColor_Red_A->setStyleSheet(strStyle);
        else if(0x2A == m_nDisplay)
            ui->button_displayColor_Green_A->setStyleSheet(strStyle);
        else if(0x3A == m_nDisplay)
            ui->button_displayColor_Yellow_A->setStyleSheet(strStyle);
        else if(0x4A == m_nDisplay)
            ui->button_displayColor_Blue_A->setStyleSheet(strStyle);
        else if(0x1B == m_nDisplay)
            ui->button_displayColor_Red_B->setStyleSheet(strStyle);
        else if(0x2B == m_nDisplay)
            ui->button_displayColor_Green_B->setStyleSheet(strStyle);
        else if(0x3B == m_nDisplay)
            ui->button_displayColor_Yellow_B->setStyleSheet(strStyle);
        else if(0x4B == m_nDisplay)
            ui->button_displayColor_Blue_B->setStyleSheet(strStyle);
        break;
    case 304:
        if(0xA == m_nVolume)
            ui->button_volumeTest_A->setStyleSheet(strStyle);
        else if(0xB == m_nVolume)
            ui->button_volumeTest_B->setStyleSheet(strStyle);
        break;
    case 305:
        if(0xA == m_nRadar)
            ui->button_radarTest_A->setStyleSheet(strStyle);
        else if(0xB == m_nRadar)
            ui->button_radarTest_B->setStyleSheet(strStyle);
        break;
    case 306:
        if(0x1A == m_nRail)
            ui->button_railingUp_A->setStyleSheet(strStyle);
        else if(0x0A == m_nRail)
            ui->button_railingDown_A->setStyleSheet(strStyle);
        else if(0x1B == m_nRail)
            ui->button_railingUp_B->setStyleSheet(strStyle);
        else if(0x0B == m_nRail)
            ui->button_railingDown_B->setStyleSheet(strStyle);
        break;
    case 307:
        if(0x01A == m_nLight)
            ui->button_frontLightOpen_A->setStyleSheet(strStyle);
        else if(0x00A == m_nLight)
            ui->button_frontLightClose_A->setStyleSheet(strStyle);
        else if(0x11A == m_nLight)
            ui->button_besideLightOpen_A->setStyleSheet(strStyle);
        else if(0x10A == m_nLight)
            ui->button_besideLightClose_A->setStyleSheet(strStyle);
        else if(0x21A == m_nLight)
            ui->button_backLightOpen_A->setStyleSheet(strStyle);
        else if(0x20A == m_nLight)
            ui->button_backLightClose_A->setStyleSheet(strStyle);
        else if(0x01B == m_nLight)
            ui->button_frontLightOpen_B->setStyleSheet(strStyle);
        else if(0x00B == m_nLight)
            ui->button_frontLightClose_B->setStyleSheet(strStyle);
        else if(0x11B == m_nLight)
            ui->button_besideLightOpen_B->setStyleSheet(strStyle);
        else if(0x10B == m_nLight)
            ui->button_besideLightClose_B->setStyleSheet(strStyle);
        else if(0x21B == m_nLight)
            ui->button_backLightOpen_B->setStyleSheet(strStyle);
        else if(0x20B == m_nLight)
            ui->button_backLightClose_B->setStyleSheet(strStyle);
        break;
    case 401:
        if(0x1A == m_nRobotDisplay)
            ui->button_displayColor_Red_Entry->setStyleSheet(strStyle);
        else if(0x2A == m_nRobotDisplay)
            ui->button_displayColor_Green_Entry->setStyleSheet(strStyle);
        else if(0x3A == m_nRobotDisplay)
            ui->button_displayColor_Yellow_Entry->setStyleSheet(strStyle);
        else if(0x4A == m_nRobotDisplay)
            ui->button_displayColor_Blue_Entry->setStyleSheet(strStyle);
        else if(0x1D == m_nRobotDisplay)
            ui->button_displayColor_Red_Exit->setStyleSheet(strStyle);
        else if(0x2D == m_nRobotDisplay)
            ui->button_displayColor_Green_Exit->setStyleSheet(strStyle);
        else if(0x3D == m_nRobotDisplay)
            ui->button_displayColor_Yellow_Exit->setStyleSheet(strStyle);
        else if(0x4D == m_nRobotDisplay)
            ui->button_displayColor_Blue_Exit->setStyleSheet(strStyle);
        break;
    case 402:
        if(0x11A == m_nCtrlLight)
            ui->button_ctrlLight1Open_Entry->setStyleSheet(strStyle);
        else if(0x10A == m_nCtrlLight)
            ui->button_ctrlLight1Close_Entry->setStyleSheet(strStyle);
        else if(0x21A == m_nCtrlLight)
            ui->button_ctrlLight2Open_Entry->setStyleSheet(strStyle);
        else if(0x20A == m_nCtrlLight)
            ui->button_ctrlLight2Close_Entry->setStyleSheet(strStyle);
        else if(0x31A == m_nCtrlLight)
            ui->button_ctrlLight3Open_Entry->setStyleSheet(strStyle);
        else if(0x30A == m_nCtrlLight)
            ui->button_ctrlLight3Close_Entry->setStyleSheet(strStyle);
        else if(0x11D == m_nCtrlLight)
            ui->button_ctrlLight1Open_Exit->setStyleSheet(strStyle);
        else if(0x10D == m_nCtrlLight)
            ui->button_ctrlLight1Close_Exit->setStyleSheet(strStyle);
        else if(0x21D == m_nCtrlLight)
            ui->button_ctrlLight2Open_Exit->setStyleSheet(strStyle);
        else if(0x20D == m_nCtrlLight)
            ui->button_ctrlLight2Close_Exit->setStyleSheet(strStyle);
        else if(0x31D == m_nCtrlLight)
            ui->button_ctrlLight3Open_Exit->setStyleSheet(strStyle);
        else if(0x30D == m_nCtrlLight)
            ui->button_ctrlLight3Close_Exit->setStyleSheet(strStyle);
        break;
    case 403:
        if(0x1A == m_nNet)
            ui->button_to4GNet_Entry->setStyleSheet(strStyle);
        else if(0x0A == m_nNet)
            ui->button_toLineNet_Entry->setStyleSheet(strStyle);
        else if(0x1D == m_nNet)
            ui->button_to4GNet_Exit->setStyleSheet(strStyle);
        else if(0x0D == m_nNet)
            ui->button_toLineNet_Exit->setStyleSheet(strStyle);
        break;
    case 404:
        if(0x0A == m_nAudio)
            ui->button_audioTest_Entry->setStyleSheet(strStyle);
        else if(0x1A == m_nAudio)
            ui->button_record_Entry->setStyleSheet(strStyle);
        else if(0x2A == m_nAudio)
            ui->button_playRecord_Entry->setStyleSheet(strStyle);
        else if(0x0D == m_nAudio)
            ui->button_audioTest_Exit->setStyleSheet(strStyle);
        else if(0x1D == m_nAudio)
            ui->button_record_Exit->setStyleSheet(strStyle);
        else if(0x2D == m_nAudio)
            ui->button_playRecord_Exit->setStyleSheet(strStyle);
        break;
    case 405:
        if(0x11A == m_nCamera)
            ui->button_openUpCamera_Entry->setStyleSheet(strStyle);
        else if(0x10A == m_nCamera)
            ui->button_closeUpCamera_Entry->setStyleSheet(strStyle);
        else if(0x01A == m_nCamera)
            ui->button_openDownCamera_Entry->setStyleSheet(strStyle);
        else if(0x00A == m_nCamera)
            ui->button_closeDownCamera_Entry->setStyleSheet(strStyle);
        else if(0x11D == m_nCamera)
            ui->button_openUpCamera_Exit->setStyleSheet(strStyle);
        else if(0x10D == m_nCamera)
            ui->button_closeUpCamera_Exit->setStyleSheet(strStyle);
        else if(0x01D == m_nCamera)
            ui->button_openDownCamera_Exit->setStyleSheet(strStyle);
        else if(0x00D == m_nCamera)
            ui->button_closeDownCamera_Exit->setStyleSheet(strStyle);
        break;
    case 406:
        if(jsonObj.contains("nCmd"))
            ui->textBrowser_log->append("nCmd_406: " + QString::number(jsonObj["nCmd"].toInt()));

        if(0x0A == m_nTelescopicArm)
            ui->button_telescopicArmInit_Entry->setStyleSheet(strStyle);
        else if(0x1A == m_nTelescopicArm)
            ui->button_telescopicArmTake10_Entry->setStyleSheet(strStyle);
        else if(0x2A == m_nTelescopicArm)
            ui->button_telescopicArmTake20_Entry->setStyleSheet(strStyle);
        else if(0x3A == m_nTelescopicArm)
            ui->button_telescopicArmBack_Entry->setStyleSheet(strStyle);
        else if(0x0D == m_nTelescopicArm)
            ui->button_telescopicArmInit_Exit->setStyleSheet(strStyle);
        else if(0x1D == m_nTelescopicArm)
            ui->button_telescopicArmTake10_Exit->setStyleSheet(strStyle);
        else if(0x2D == m_nTelescopicArm)
            ui->button_telescopicArmTake20_Exit->setStyleSheet(strStyle);
        else if(0x3D == m_nTelescopicArm)
            ui->button_telescopicArmBack_Exit->setStyleSheet(strStyle);
        break;
    case 407:
        if(0x01A == m_nReader)
            ui->button_takeETCAntenna_Entry->setStyleSheet(strStyle);
        else if(0x00A == m_nReader)
            ui->button_takeCPCAntenna_Entry->setStyleSheet(strStyle);
        else if(0x14A == m_nReader)
            ui->button_upSlotWriteCard_Entry->setStyleSheet(strStyle);
        else if(0x13A == m_nReader)
            ui->button_upSlotReadCard_Entry->setStyleSheet(strStyle);
        else if(0x15A == m_nReader)
            ui->button_upSlotStop_Entry->setStyleSheet(strStyle);
        else if(0x04A == m_nReader)
            ui->button_downSlotWriteCard_Entry->setStyleSheet(strStyle);
        else if(0x03A == m_nReader)
            ui->button_downSlotReadCard_Entry->setStyleSheet(strStyle);
        else if(0x05A == m_nReader)
            ui->button_downSlotStop_Entry->setStyleSheet(strStyle);
        else if(0x01D == m_nReader)
            ui->button_takeETCAntenna_Exit->setStyleSheet(strStyle);
        else if(0x00D == m_nReader)
            ui->button_takeCPCAntenna_Exit->setStyleSheet(strStyle);
        else if(0x14D == m_nReader)
            ui->button_upSlotWriteCard_Exit->setStyleSheet(strStyle);
        else if(0x13D == m_nReader)
            ui->button_upSlotReadCard_Exit->setStyleSheet(strStyle);
        else if(0x15D == m_nReader)
            ui->button_upSlotStop_Exit->setStyleSheet(strStyle);
        else if(0x04D == m_nReader)
            ui->button_downSlotWriteCard_Exit->setStyleSheet(strStyle);
        else if(0x03D == m_nReader)
            ui->button_downSlotReadCard_Exit->setStyleSheet(strStyle);
        else if(0x05D == m_nReader)
            ui->button_downSlotStop_Exit->setStyleSheet(strStyle);
        break;
    case 408:
        if(jsonObj.contains("nCmd"))
            ui->textBrowser_log->append("nCmd_408: " + QString::number(jsonObj["nCmd"].toInt()));

        if(00 == m_nEntryMachine)
            ui->button_robotModeCardMachine->setStyleSheet(strStyle);
        else if(10 == m_nEntryMachine)
            ui->button_sentryboxModeCardMahine->setStyleSheet(strStyle);
        else if(20 == m_nEntryMachine)
            ui->button_upleftSlotInit->setStyleSheet(strStyle);
        else if(21 == m_nEntryMachine)
            ui->button_upleftSlotPrepareCard->setStyleSheet(strStyle);
        else if(22 == m_nEntryMachine)
            ui->button_upleftSlotSendCard->setStyleSheet(strStyle);
        else if(23 == m_nEntryMachine)
            ui->button_upleftSlotGetCard->setStyleSheet(strStyle);
        else if(24 == m_nEntryMachine)
            ui->button_upleftSlotRecoverCard->setStyleSheet(strStyle);
        else if(25 == m_nEntryMachine)
            ui->button_upleftSlotSetCardNum->setStyleSheet(strStyle);
        else if(30 == m_nEntryMachine)
            ui->button_uprightSlotInit->setStyleSheet(strStyle);
        else if(31 == m_nEntryMachine)
            ui->button_uprightSlotPrepareCard->setStyleSheet(strStyle);
        else if(32 == m_nEntryMachine)
            ui->button_uprightSlotSendCard->setStyleSheet(strStyle);
        else if(33 == m_nEntryMachine)
            ui->button_uprightSlotGetCard->setStyleSheet(strStyle);
        else if(34 == m_nEntryMachine)
            ui->button_uprightSlotRecoverCard->setStyleSheet(strStyle);
        else if(35 == m_nEntryMachine)
            ui->button_uprightSlotSetCardNum->setStyleSheet(strStyle);
        else if(40 == m_nEntryMachine)
            ui->button_downleftSlotInit->setStyleSheet(strStyle);
        else if(41 == m_nEntryMachine)
            ui->button_downleftSlotPrepareCard->setStyleSheet(strStyle);
        else if(42 == m_nEntryMachine)
            ui->button_downleftSlotSendCard->setStyleSheet(strStyle);
        else if(43 == m_nEntryMachine)
            ui->button_downleftSlotGetCard->setStyleSheet(strStyle);
        else if(44 == m_nEntryMachine)
            ui->button_downleftSlotRecoverCard->setStyleSheet(strStyle);
        else if(45 == m_nEntryMachine)
            ui->button_downleftSlotSetCardNum->setStyleSheet(strStyle);
        else if(50 == m_nEntryMachine)
            ui->button_downrightSlotInit->setStyleSheet(strStyle);
        else if(51 == m_nEntryMachine)
            ui->button_downrightSlotPrepareCard->setStyleSheet(strStyle);
        else if(52 == m_nEntryMachine)
            ui->button_downrightSlotSendCard->setStyleSheet(strStyle);
        else if(53 == m_nEntryMachine)
            ui->button_downrightSlotGetCard->setStyleSheet(strStyle);
        else if(54 == m_nEntryMachine)
            ui->button_downrightSlotRecoverCard->setStyleSheet(strStyle);
        else if(55 == m_nEntryMachine)
            ui->button_downrightSlotSetCardNum->setStyleSheet(strStyle);
        break;
    case 409:
        if(jsonObj.contains("nCmd"))
            ui->textBrowser_log->append("nCmd_409: " + QString::number(jsonObj["nCmd"].toInt()));
        if(00 == m_nExitMachine)
            ui->button_upSlotInit->setStyleSheet(strStyle);
        else if(01 == m_nExitMachine)
            ui->button_upSlotSetStatus->setStyleSheet(strStyle);
        else if(02 == m_nExitMachine)
            ui->button_upSlotBackCard->setStyleSheet(strStyle);
        else if(03 == m_nExitMachine)
            ui->button_upSlotRecoverCard->setStyleSheet(strStyle);
        else if(10 == m_nExitMachine)
            ui->button_downSlotInit->setStyleSheet(strStyle);
        else if(11 == m_nExitMachine)
            ui->button_downSlotSetStatus->setStyleSheet(strStyle);
        else if(12 == m_nExitMachine)
            ui->button_downSlotBackCard->setStyleSheet(strStyle);
        else if(13 == m_nExitMachine)
            ui->button_downSlotRecoverCard->setStyleSheet(strStyle);
        break;
    case 411:
        if(0x11A == m_nRobotRelay)
            ui->button_relay1Open_Entry->setStyleSheet(strStyle);
        else if(0x10A == m_nRobotRelay)
            ui->button_relay1Close_Entry->setStyleSheet(strStyle);
        else if(0x21A == m_nRobotRelay)
            ui->button_relay2Open_Entry->setStyleSheet(strStyle);
        else if(0x20A == m_nRobotRelay)
            ui->button_relay2Close_Entry->setStyleSheet(strStyle);
        else if(0x31A == m_nRobotRelay)
            ui->button_relay3Open_Entry->setStyleSheet(strStyle);
        else if(0x30A == m_nRobotRelay)
            ui->button_relay3Close_Entry->setStyleSheet(strStyle);
        else if(0x41A == m_nRobotRelay)
            ui->button_relay4Open_Entry->setStyleSheet(strStyle);
        else if(0x40A == m_nRobotRelay)
            ui->button_relay4Close_Entry->setStyleSheet(strStyle);
        else if(0x11D == m_nRobotRelay)
            ui->button_relay1Open_Exit->setStyleSheet(strStyle);
        else if(0x10D == m_nRobotRelay)
            ui->button_relay1Close_Exit->setStyleSheet(strStyle);
        else if(0x21D == m_nRobotRelay)
            ui->button_relay2Open_Exit->setStyleSheet(strStyle);
        else if(0x20D == m_nRobotRelay)
            ui->button_relay2Close_Exit->setStyleSheet(strStyle);
        else if(0x31D == m_nRobotRelay)
            ui->button_relay3Open_Exit->setStyleSheet(strStyle);
        else if(0x30D == m_nRobotRelay)
            ui->button_relay3Close_Exit->setStyleSheet(strStyle);
        else if(0x41D == m_nRobotRelay)
            ui->button_relay4Open_Exit->setStyleSheet(strStyle);
        else if(0x40D == m_nRobotRelay)
            ui->button_relay4Close_Exit->setStyleSheet(strStyle);
        break;
    case 412:
        if(0x1A == m_nScaner)
            ui->button_openScaner_Entry->setStyleSheet(strStyle);
        else if(0x0A == m_nScaner)
            ui->button_closeScaner_Entry->setStyleSheet(strStyle);
        else if(0x1D == m_nScaner)
            ui->button_openScaner_Exit->setStyleSheet(strStyle);
        else if(0x0D == m_nScaner)
            ui->button_closeScaner_Exit->setStyleSheet(strStyle);
        break;
    case 413:
        if(00 == m_nEntryBurn)
            ui->button_robotModeBurn->setStyleSheet(strStyle);
        else if(10 == m_nEntryBurn)
            ui->button_sentryboxModeBurn->setStyleSheet(strStyle);
        else if(20 == m_nEntryBurn)
            ui->button_upleftSlotInit_Burn->setStyleSheet(strStyle);
        else if(21 == m_nEntryBurn)
            ui->button_upleftSlotOpenReader->setStyleSheet(strStyle);
        else if(23 == m_nEntryBurn)
            ui->button_upleftSlotSetCardNum_Burn->setStyleSheet(strStyle);
        else if(24 == m_nEntryBurn)
            ui->button_upleftSlotStartBurn->setStyleSheet(strStyle);
        else if(25 == m_nEntryBurn)
            ui->button_upleftSlotStopBurn->setStyleSheet(strStyle);
        else if(30 == m_nEntryBurn)
            ui->button_uprightSlotInit_Burn->setStyleSheet(strStyle);
        else if(31 == m_nEntryBurn)
            ui->button_uprightSlotOpenReader->setStyleSheet(strStyle);
        else if(33 == m_nEntryBurn)
            ui->button_uprightSlotSetCardNum_Burn->setStyleSheet(strStyle);
        else if(34 == m_nEntryBurn)
            ui->button_uprightSlotStartBurn->setStyleSheet(strStyle);
        else if(35 == m_nEntryBurn)
            ui->button_uprightSlotStopBurn->setStyleSheet(strStyle);
        else if(40 == m_nEntryBurn)
            ui->button_downleftSlotInit_Burn->setStyleSheet(strStyle);
        else if(41 == m_nEntryBurn)
            ui->button_downleftSlotOpenReader->setStyleSheet(strStyle);
        else if(43 == m_nEntryBurn)
            ui->button_downleftSlotSetCardNum_Burn->setStyleSheet(strStyle);
        else if(44 == m_nEntryBurn)
            ui->button_downleftSlotStartBurn->setStyleSheet(strStyle);
        else if(45 == m_nEntryBurn)
            ui->button_downleftSlotStopBurn->setStyleSheet(strStyle);
        else if(50 == m_nEntryBurn)
            ui->button_downrightSlotInit_Burn->setStyleSheet(strStyle);
        else if(51 == m_nEntryBurn)
            ui->button_downrightSlotOpenReader->setStyleSheet(strStyle);
        else if(53 == m_nEntryBurn)
            ui->button_downrightSlotSetCardNum_Burn->setStyleSheet(strStyle);
        else if(54 == m_nEntryBurn)
            ui->button_downrightSlotStartBurn->setStyleSheet(strStyle);
        else if(55 == m_nEntryBurn)
            ui->button_downrightSlotStopBurn->setStyleSheet(strStyle);
        break;
    case 414:
        if(00 == m_nExitBurn)
            ui->button_upSlotInit_Burn->setStyleSheet(strStyle);
        else if(01 == m_nExitBurn)
            ui->button_upSlotOpenReader->setStyleSheet(strStyle);
        else if(02 == m_nExitBurn)
            ui->button_upSlotInitReader->setStyleSheet(strStyle);
        else if(03 == m_nExitBurn)
            ui->button_upSlotStartBurn->setStyleSheet(strStyle);
        else if(04 == m_nExitBurn)
            ui->button_upSlotStopBurn->setStyleSheet(strStyle);
        else if(10 == m_nExitBurn)
            ui->button_downSlotInit_Burn->setStyleSheet(strStyle);
        else if(11 == m_nExitBurn)
            ui->button_downSlotOpenReader->setStyleSheet(strStyle);
        else if(12 == m_nExitBurn)
            ui->button_downSlotInitReader->setStyleSheet(strStyle);
        else if(13 == m_nExitBurn)
            ui->button_downSlotStartBurn->setStyleSheet(strStyle);
        else if(14 == m_nExitBurn)
            ui->button_downSlotStopBurn->setStyleSheet(strStyle);
        break;
    default:
        break;
    }

    return true;
}











